#include "klibc/mmap.hpp"
#include "klibc/algorithm.hpp"
#include "klibc/function.hpp"

#include "drivers/screen.hpp"

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
		if (!(address >= map->e820Entries[i].baseAddress)) continue;

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

	// If a memory block large enough was found, make sure it is not in an
	// unusable range.
	if (inRange)
	{
		// Reiterate over map to ensure region isn't in an unusable range.
		// if type != 1, dont use memory, even if it is in another range that is
		// type 1.
		for (uint32_t i = 0; i < map->e820EntryCount; i++)
		{
			// We already know the block is contained in a type 1 region, skip
			// any entries that type 1.
			if (map->e820Entries[i].type == 1) continue;
			if (!(map->e820Entries[i].baseAddress >= address)) continue;

			if (address <= map->e820Entries[i].baseAddress +
					map->e820Entries[i].length &&
					map->e820Entries[i].baseAddress <= address + length)
			{
				inRange = false;
				break;
			}
		}
	}

	return inRange;
}

