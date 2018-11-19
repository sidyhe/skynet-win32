#pragma once
#include <fcntl.h>
#include <process.h>
#include <WinSock2.h>

#include <pint.h>

typedef int pid_t;
typedef int useconds_t;

enum { LOCK_EX, LOCK_NB };

int close(int fd);
int read(int fd, void *buffer, size_t sz);
int write(int fd, const void *ptr, size_t sz);

void sleep(size_t ms);
int usleep(useconds_t usec);

int random();
void srandom(unsigned int seed);

int pipe(int fd[2]);
int kill(pid_t pid, int exit_code);

int flock(int fd, int flag);
int daemon(int a, int b);
