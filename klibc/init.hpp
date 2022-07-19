#ifndef _KLIBC_INIT_HPP
#define _KLIBC_INIT_HPP

#include "klibc/malloc_internal.hpp"

inline void initKlibc()
{
	initMalloc();
}

#endif

