#pragma once

int clock_gettime(int what, struct timespec *ti);
enum { CLOCK_THREAD_CPUTIME_ID, CLOCK_REALTIME, CLOCK_MONOTONIC };
