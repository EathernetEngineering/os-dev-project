#ifndef _KLIBC_MEMORY_HPP
#define _KLIBC_MEMORY_HPP

#include <stddef.h>
#include "klibc/malloc_internal.hpp"

void *kmalloc(size_t size);

#define alloca(size) __builtin_alloca(size)

#define container_of(ptr, type, member) ({ \
		const typeof(((type*)0)->member)* __mptr = (ptr); \
		(type*)((char*)__mptr - offsetof(type, member));})

#endif

