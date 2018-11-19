#include "unistd.h"
#include "psignal.h"
#include <conio.h>

NTSYSAPI NTSTATUS NTAPI NtDelayExecution(IN BOOLEAN Alertable, IN PLARGE_INTEGER DelayInterval);
typedef enum _THREADINFOCLASS {
    ThreadBasicInformation,
    ThreadTimes,
    ThreadPriority,
    ThreadBasePriority,
    ThreadAffinityMask,
    ThreadImpersonationToken,
    ThreadDescriptorTableEntry,
    ThreadEnableAlignmentFaultFixup,
    ThreadEventPair_Reusable,
    ThreadQuerySetWin32StartAddress,
    ThreadZeroTlsCell,
    ThreadPerformanceCount,
    ThreadAmILastThread,
    ThreadIdealProcessor,
    ThreadPriorityBoost,
    ThreadSetTlsArrayAddress,
    ThreadIsIoPending,
    ThreadHideFromDebugger,
    ThreadBreakOnTermination,
    ThreadSwitchLegacyState,
    ThreadIsTerminated,
    MaxThreadInfoClass
} THREADINFOCLASS;
NTSYSCALLAPI
NTSTATUS
NTAPI
NtQueryInformationThread (
    __in HANDLE ThreadHandle,
    __in THREADINFOCLASS ThreadInformationClass,
    __out_bcount(ThreadInformationLength) PVOID ThreadInformation,
    __in ULONG ThreadInformationLength,
    __out_opt PULONG ReturnLength
    );
typedef struct _KERNEL_USER_TIMES {
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER ExitTime;
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
} KERNEL_USER_TIMES;
typedef KERNEL_USER_TIMES *PKERNEL_USER_TIMES;

int read_console(char* ptr, size_t size)
{
	size_t i = 0;

	while (i < size)
	{
		if (!_kbhit())
			break;
		const int c = _getch();

		if (!c)
			continue;

		ptr[i++] = (c & 0xFF);
		_putch(c);

		if (c == '\r')
		{
			_putch('\n');
			ptr[i - 1] = '\n';
		}
	}

	return (int)i;
}

int read(int fd, void *buffer, size_t sz)
{
	if (fd == 0) {
		return read_console((char*)buffer, sz);
	}

	WSABUF vecs[1];
	vecs[0].buf = buffer;
	vecs[0].len = sz;

	DWORD bytesRecv = 0;
	DWORD flags = 0;
	if (WSARecv(fd, vecs, 1, &bytesRecv, &flags, NULL, NULL)) {
		if (WSAGetLastError() == WSAECONNRESET)
			return 0;
		return -1;
	}
	else
		return bytesRecv;
}

int write(int fd, const void *ptr, size_t sz)
{
	WSABUF wb;
	DWORD NumberOfBytesSent;

	wb.len = sz;
	wb.buf = (char*)ptr;

	if (WSASend(fd, &wb, 1, &NumberOfBytesSent, 0, NULL, NULL))
		return -1;
	else
		return NumberOfBytesSent;
}

int close(int fd)
{
	shutdown(fd, SD_BOTH);
	return closesocket(fd);
}

void sleep(size_t ms)
{
	SleepEx((DWORD)ms, FALSE);
}

int usleep(useconds_t usec)
{
	if (usec == 0)
	{
		SleepEx(0, FALSE);
	}
	else if ((usec < 0) || (usec >= 1000000))
	{
		return EINVAL;
	}
	else
	{
		LARGE_INTEGER li;
		li.QuadPart = -1 * usec * 10;
		NtDelayExecution(FALSE, &li);
	}

	return 0;
}

int random() { return rand(); }
void srandom(unsigned int seed) { srand(seed); }

void _socket_keepalive(SOCKET fd) {
	int keepalive = 1;
	setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive));
}

int pipe(int fd[2])
{
	int sal;
	struct sockaddr_in sa;
	SOCKET listen_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sa.sin_port = 0;
	sa.sin_family = AF_INET;
	sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (bind(listen_fd, (struct sockaddr*)&sa, sizeof(sa)) != 0)
	{
		closesocket(listen_fd);
		return EBUSY;
	}

	if (listen(listen_fd, 5) != 0)
	{
		closesocket(listen_fd);
		return EAGAIN;
	}

	sal = sizeof(sa);
	getsockname(listen_fd, (struct sockaddr*)&sa, &sal);

	SOCKET fd_a = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (connect(fd_a, (struct sockaddr*)&sa, sizeof(sa)) != 0) {
		closesocket(fd_a);
		closesocket(listen_fd);
		return -1;
	}

	sal = sizeof(sa);
	SOCKET fd_b = accept(listen_fd, (struct sockaddr*)&sa, &sal);

	if (fd_b == INVALID_SOCKET)
	{
		closesocket(fd_a);
		closesocket(listen_fd);
		return -1;
	}

	fd[0] = (int)fd_b;
	fd[1] = (int)fd_a;

	closesocket(listen_fd);

	_socket_keepalive(fd_a);
	_socket_keepalive(fd_b);

	return 0;
}

int kill(pid_t pid, int exit_code)
{
	int hr;
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

	if (!hProc)
	{
		return EACCES;
	}

	if (TerminateProcess(hProc, exit_code))
	{
		hr = 0;
	}
	else
	{
		hr = GetLastError();
	}

	return hr;
}

int flock(int fd, int flag)
{
	return EINVAL;
}

int daemon(int a, int b)
{
	return EINVAL;
}

void sigfillset(int *flag)
{

}

int sigemptyset(sigset_t *set)
{
	return EINVAL;
}

void sigaction(int flag, struct sigaction *action, int param)
{

}

char* strsep(char **stringp, const char *delim)
{
	char *s;
	const char *spanp;
	int c, sc;
	char *tok;
	if ((s = *stringp) == NULL)
		return (NULL);
	for (tok = s;;) {
		c = *s++;
		spanp = delim;
		do {
			if ((sc = *spanp++) == c) {
				if (c == 0)
					s = NULL;
				else
					s[-1] = 0;
				*stringp = s;
				return (tok);
			}
		} while (sc != 0);
	}
	/* NOTREACHED */

	return 0;
}

#include <time.h>
#include "ptime.h"
int clock_gettime(int what, struct timespec *ti)
{
	switch (what)
	{
	case CLOCK_REALTIME:
		{
			ULARGE_INTEGER uli;

			GetSystemTimeAsFileTime((LPFILETIME)&uli);

			ti->tv_sec = uli.QuadPart / (10 * 1000 * 1000);
			ti->tv_nsec = (uli.QuadPart % (10 * 1000 * 1000)) * 100;
		}
		break;
	case CLOCK_MONOTONIC:
		{
			DWORD64 tick = GetTickCount64();

			ti->tv_sec = tick / 1000;
			ti->tv_nsec = (tick % 1000) * 1000;
		}
		break;
	case CLOCK_THREAD_CPUTIME_ID:
		{
			NTSTATUS st;
			ULONG ReturnLength;
			KERNEL_USER_TIMES kut;

			st = NtQueryInformationThread(GetCurrentThread(), ThreadTimes, &kut, sizeof(kut), &ReturnLength);
			if (st != 0)
				return EPERM;

			ti->tv_sec = kut.UserTime.QuadPart / (10 * 1000 * 1000);
			ti->tv_nsec = (kut.UserTime.QuadPart % (10 * 1000 * 1000)) * 100;
		}
		break;
	default:
		__debugbreak();
		return EPERM;
	}

	return 0;
}
