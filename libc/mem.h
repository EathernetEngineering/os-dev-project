#ifndef __LIBC__MEM_H__
#define __LIBC__MEM_H__

#include <libc/stddef.h>

void* memcpy(void* dest, const void* src, unsigned long length);
void* memset(void* dest, register int val, register long len);

uintptr_t kmalloc(size_t size);

#endif

