#ifndef _MEMORY_HPP
#define _MEMORY_HPP

#include <stddef.h>
#include "libc/string.hpp"

#define MAX_E820_ENTRIES 64

typedef struct {
	union {
		struct {
			uint64_t present               :  1;
			uint64_t readWrite             :  1;
			uint64_t userSupervisor        :  1;
			uint64_t pageWriteThrough      :  1;
			uint64_t pageCacheDisabled     :  1;
			uint64_t accessed              :  1;
			uint64_t ignored1              :  1;
			uint64_t pageSize              :  1;
			uint64_t ignored2              :  4;
			uint64_t pageFrameNumber       : 36;
			uint64_t reserved              :  4;
			uint64_t ignored3              : 11;
			uint64_t executeDisable        :  1;
		};
		uint64_t value;
	};
} Pml4e;

typedef struct {
	union {
		struct {
			uint64_t present               :  1;
			uint64_t readWrite             :  1;
			uint64_t userSupervisor        :  1;
			uint64_t pageWriteThrough      :  1;
			uint64_t pageCacheDisabled     :  1;
			uint64_t accessed              :  1;
			uint64_t ignored1              :  1;
			uint64_t pageSize              :  1;
			uint64_t ignored2              :  4;
			uint64_t pageFrameNumber       : 36;
			uint64_t reserved              :  4;
			uint64_t ignored3              : 11;
			uint64_t executeDisable        :  1;
		};
		uint64_t value;
	};
} Pdpte;

typedef struct {
	union {
		struct {
			uint64_t present               :  1;
			uint64_t readWrite             :  1;
			uint64_t userSupervisor        :  1;
			uint64_t pageWriteThrough      :  1;
			uint64_t pageCacheDisabled     :  1;
			uint64_t accessed              :  1;
			uint64_t ignored1              :  1;
			uint64_t pageSize              :  1;
			uint64_t ignored2              :  4;
			uint64_t pageFrameNumber       : 36;
			uint64_t reserved              :  4;
			uint64_t ignored3              : 11;
			uint64_t executeDisable        :  1;
		};
		uint64_t value;
	};
} Pde;

typedef struct {
	union {
		struct {
			uint64_t present               :  1;
			uint64_t readWrite             :  1;
			uint64_t userSupervisor        :  1;
			uint64_t pageWriteThrough      :  1;
			uint64_t pageCacheDisabled     :  1;
			uint64_t accessed              :  1;
			uint64_t dirty                 :  1;
			uint64_t pageAttributeTable    :  1;
			uint64_t global                :  1;
			uint64_t ignored1              :  3;
			uint64_t pageFrameNumber       : 36;
			uint64_t reserved              :  4;
			uint64_t ignored2              : 11;
			uint64_t executeDisable        :  1;
		};
		uint64_t value;
	};
} Pte;

#define MAX_PAGE_TABLE_ENTRIES 512

typedef struct {
	Pml4e value[MAX_PAGE_TABLE_ENTRIES];
} __attribute__((aligned(0x1000))) PageMapLevel4;

typedef struct {
	Pdpte value[MAX_PAGE_TABLE_ENTRIES];
} __attribute__((aligned(0x1000))) PageDirectoryPointerTable;

typedef struct {
	Pde value[MAX_PAGE_TABLE_ENTRIES];
} __attribute__((aligned(0x1000))) PageDirectory;

typedef struct {
	Pte value[MAX_PAGE_TABLE_ENTRIES];
} __attribute__((aligned(0x1000))) PageTable;

typedef struct {
	uint32_t baseAddressLow;
	uint32_t baseAddressHigh;
	uint32_t lengthLow;
	uint32_t lengthHigh;
	uint32_t type;
	uint32_t extendedAttributes;
} __attribute__((__packed__)) MemoryDescriptor;

typedef struct _MemoryMap {
	uint16_t blockAbove1M;
	uint16_t blocksAbove1MUnder15M;
	uint16_t chunksAbove16M;
	uint32_t numE820Entries;
	MemoryDescriptor e820Entries[MAX_E820_ENTRIES];

	_MemoryMap()
	{
		memcpy(this, (void*)0x500, sizeof(_MemoryMap));
	}
} __attribute__((packed)) MemoryMap;

void setupPaging();

void *kmalloc(size_t size);

#define container_of(ptr, type, member) ({ \
		const typeof(((type*)0)->member)* __mptr = (ptr); \
		(type*)((char*)__mptr - offsetof(type, member));})

#endif

