#ifndef _FUNCTION_H
#define _FUNCTION_H

#include "libc/string.hpp"

#define UNUSED(param) (void)(param)
#define ZeroMemory(ptr, length) memset(ptr, 0, length)

template<typename T>
inline void swap(T *first, T *second)
{
	T tmp = *first;
	*first = *second;
	*second = tmp;
}

int32_t *sort(int32_t *array, size_t count);
uint32_t *sort(uint32_t *array, size_t count);
int64_t *sort(int64_t *array, size_t count);
uint64_t *sort(uint64_t *array, size_t count);

#endif

