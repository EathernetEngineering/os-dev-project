#ifndef _KLIBC_INIT_HPP
#define _KLIBC_INIT_HPP

#include "klibc/malloc_internal.hpp"

#include "drivers/screen.hpp"

inline void initKlibc()
{
	kprint("Initalizing klibc\n");

	initMalloc();
}

#endif

