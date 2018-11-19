#pragma once

void __sync_synchronize();

int __sync_add_and_fetch(int *p, int n);
int __sync_sub_and_fetch(int *p, int n);
int __sync_fetch_and_add(int *p, int n);
int __sync_fetch_and_sub(int *p, int n);

int __sync_and_and_fetch(int *p, int n);
char __sync_int_compare_and_swap(int *ptr, int oval, int nval);
char __sync_short_compare_and_swap(short *ptr, short oval, short nval);
char __sync_pointer_compare_and_swap(void **ptr, void *oval, void *nval);

void __sync_lock_release(int *p);
int __sync_lock_test_and_set(int *p, int n);
