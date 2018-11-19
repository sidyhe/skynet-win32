#include "gcc.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

void __sync_synchronize()
{
	static LONG v = 0;

	InterlockedAdd(&v, 1);
}

int __sync_add_and_fetch(int *p, int n)
{
	return InterlockedAdd(p, n);
}

int __sync_sub_and_fetch(int *p, int n)
{
	return InterlockedAdd(p, 0 - n);
}

int __sync_fetch_and_add(int *p, int n)
{
	return InterlockedExchangeAdd(p, n);
}

int __sync_fetch_and_sub(int *p, int n)
{
	return InterlockedExchangeAdd(p, 0 - n);
}

int __sync_and_and_fetch(int *p, int n)
{
	return (InterlockedAnd(p, n) & n);
}

char __sync_int_compare_and_swap(int *ptr, int oval, int nval)
{
	return (InterlockedCompareExchange(ptr, nval, oval) == oval);
}

char __sync_short_compare_and_swap(short *ptr, short oval, short nval)
{
	return (InterlockedCompareExchange16(ptr, nval, oval) == oval);
}

char __sync_pointer_compare_and_swap(void **ptr, void *oval, void *nval)
{
	return (InterlockedCompareExchangePointer(ptr, nval, oval) == oval);
}

void __sync_lock_release(int *p)
{
	InterlockedExchange(p, 0);
}

int __sync_lock_test_and_set(int *p, int n)
{
	return InterlockedExchange(p, n);
}
