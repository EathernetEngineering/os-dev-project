#include "libc/ctype.hpp"

int isalpha(uint8_t c)
{
	return (unsigned)(c - 0x41) <= (0x5A - 0x41) ? c : 0;
}

