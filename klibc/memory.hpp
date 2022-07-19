#ifndef _KLIBC_MEMORY_HPP
#define _KLIBC_MEMORY_HPP

#include <stddef.h>
#include "klibc/string.hpp"

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

void setupPaging();

void *kmalloc(size_t size);

#define container_of(ptr, type, member) ({ \
		const typeof(((type*)0)->member)* __mptr = (ptr); \
		(type*)((char*)__mptr - offsetof(type, member));})

#endif

