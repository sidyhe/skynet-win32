#include <cpoll.h>
#include <unistd.h>
#include <socket_poll.h>

#include <assert.h>
#include <WinSock2.h>

bool sp_invalid(int efd) {
	return efd == -1;
}

int sp_create() {
	return cpoll_create(1024);
}

void sp_release(int efd) {
	close(efd);
}

int sp_add(int efd, int sock, void *ud) {
	struct cpoll_event ev;
	ev.events = CPOLLIN;
	ev.data.ptr = ud;
	if (cpoll_ctl(efd, CPOLL_CTL_ADD, sock, &ev) == -1) {
		return 1;
	}
	return 0;
}

void sp_del(int efd, int sock) {
	cpoll_ctl(efd, CPOLL_CTL_DEL, sock, NULL);
}

void sp_write(int efd, int sock, void *ud, bool enable) {
	struct cpoll_event ev;
	ev.events = CPOLLIN | (enable ? CPOLLOUT : 0);
	ev.data.ptr = ud;
	cpoll_ctl(efd, CPOLL_CTL_MOD, sock, &ev);
}

int sp_wait(int efd, struct event *e, int max) {
	assert(max <= 1024);
	struct cpoll_event ev[1024];
	int n = cpoll_wait(efd, ev, max, -1);
	int i;
	for (i = 0; i < n; i++) {
		e[i].s = ev[i].data.ptr;
		unsigned flag = ev[i].events;
		e[i].write = (flag & CPOLLOUT) != 0;
		e[i].read = (flag & CPOLLIN) != 0;
	}

	return n;
}

void sp_nonblocking(int fd) {
	u_long ul = 1;
	ioctlsocket(fd, FIONBIO, &ul);
}
