#include "libc/mmap.hpp"

#include "kernel/kprint.hpp"
#include "libc/function.hpp"

MemoryMap *sortMemoryMap(MemoryMap *mapIn)
{
	if (mapIn == (void*)0x500) kcrit("Cannot overwrite original map\n");

	for (uint32_t pass = 0; pass < mapIn->e820EntryCount - 1; pass++)
	{
		for (uint32_t i = 0; i < mapIn->e820EntryCount - 1 - pass; i++)
		{
			if (mapIn->e820Entries[i].baseAddress >
					mapIn->e820Entries[i + 1].baseAddress)
				swap(&mapIn->e820Entries[i], &mapIn->e820Entries[i + 1]);
		}
	}

	return mapIn;
}

