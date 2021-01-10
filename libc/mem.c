#include "mem.h"
#include "function.h"
#include "stddef.h"

void bcopy(const void *src, void *dest, unsigned long len)
{
	if (dest < src)
	{
		const char *firsts = (const char*)src;
		char *firstd = (char*)dest;
		while (len--)
			*firstd++ = *firsts++;
	}
	else
	{
		const char *lasts = (const char*)src + (len-1);
		char* lastd = (char*)dest + (len-1);
		while (len--)
			*lastd-- = *lasts--;
	}
}

void* memcpy(void* out, const void* in, unsigned long length)
{
	bcopy(in, out, length);
	return out;
}

void* memset(void* dest, register int val, register long len)
{
	register unsigned char *ptr = (unsigned char*)dest;
	while (len-- > 0)
		*ptr++ = val;
	return dest;
}

unsigned kmalloc(unsigned size, int align, unsigned *physical_addr)
{
	UNUSED(size);
	UNUSED(align);
	UNUSED(physical_addr);
	return 0;
}

