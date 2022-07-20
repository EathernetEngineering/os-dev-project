#ifndef _KLIBC_MMAP_HPP
#define _KLIBC_MMAP_HPP

#include "klibc/string.hpp"
#include "klibc/function.hpp"

#define MAX_E820_ENTRIES 64

typedef struct _E820MemoryDescriptor {
	union {
		struct {
			uint32_t baseAddressLow;
			uint32_t baseAddressHigh;
		};
		uint64_t baseAddress;
	};
	union {
		struct {
			uint32_t lengthLow;
			uint32_t lengthHigh;
		};
		uint64_t length;
	};
	uint32_t type;
	uint32_t extendedAttributes;
} __attribute__((__packed__)) E820MemoryDescriptor;

typedef struct _MemoryMap {
	uint16_t blockAbove1M;
	uint16_t blocksAbove1MUnder15M;
	uint16_t chunksAbove16M;
	uint32_t e820EntryCount;
	E820MemoryDescriptor e820Entries[MAX_E820_ENTRIES];

	_MemoryMap()
	{
		memcpy(this, (void*)0x500, sizeof(_MemoryMap));
	}
} __attribute__((packed)) MemoryMap;

inline MemoryMap *fillMemoryMap(MemoryMap *mapIn)
{
	memcpy(mapIn, (void*)0x500, sizeof(MemoryMap));
	return mapIn;
}

MemoryMap *sortMemoryMap(MemoryMap *mapIn);
bool memoryInUsableRange(const MemoryMap* map, uintptr_t address, size_t length);

#endif

