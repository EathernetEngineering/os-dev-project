#include "klibc/mmap.hpp"

#include "kernel/kprint.hpp"
#include "klibc/algorithm.hpp"
#include "klibc/function.hpp"

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

bool memoryInUsableRange(
		const MemoryMap* map,
		uintptr_t address,
		size_t length)
{
	bool inRange = false;
	for (uint32_t i = 0; i < map->e820EntryCount; i++)
	{
		if (map->e820Entries[i].type != 1) continue;
		if (!(address > map->e820Entries[i].baseAddress)) continue;

		// don't bother checking if address is lower than the top of the range
		// because if address + length is lower than the top of the range
		// address must be contained within the range, and if address + lengths
		// is not in the range we can't use the block anyway.
		if (address + length <
				map->e820Entries[i].baseAddress + map->e820Entries[i].length)
		{
			inRange = true;
			break;
		}
	}
	return inRange;
}

