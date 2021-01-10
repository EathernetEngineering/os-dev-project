#ifndef __LIBC__MEM_H__
#define __LIBC__MEM_H__

void* memcpy(void* dest, const void* src, unsigned long length);
void* memset(void* dest, register int val, register long len);

unsigned kmalloc(unsigned size, int align, unsigned *physical_addr);

#endif

