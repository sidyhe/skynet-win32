#pragma once

#define SIGPIPE		0
#define	SIGHUP		1
#define SA_RESTART	2

typedef int sigset_t;

struct sigaction {
	void(*sa_handler)(int);
	int sa_flags;
	sigset_t sa_mask;
};

void sigfillset(int *flag);
int sigemptyset(sigset_t *set);
void sigaction(int flag, struct sigaction *action, int param);
