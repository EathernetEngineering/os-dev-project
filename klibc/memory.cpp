#include "klibc/memory.hpp"
#include "klibc/function.hpp"
#include "klibc/stdlib.hpp"

#include "kernel/kprint.hpp"

#include "cpu/isr.hpp"

// Allocate memory statically for second through fourth page table.
PageMapLevel4 *_pPml4;
PageDirectoryPointerTable *_pPdpt;
PageDirectory *_pPd;
PageTable *_ppPt[MAX_PAGE_TABLE_ENTRIES];

PageTable _pPt[MAX_PAGE_TABLE_ENTRIES];

void pageFaultHandler(registers_t* r);

void setupPaging()
{
	kprint("Setting ou paging.\n");

	registerInterruptHandler(14, pageFaultHandler);
	_pPml4 = (PageMapLevel4 *)(0x1000);
	_pPdpt = (PageDirectoryPointerTable *)(0x2000);
	_pPd = (PageDirectory *)(0x3000);

	_ppPt[0] = reinterpret_cast<PageTable *>(0x4000);
	for (uint32_t i = 1; i < MAX_PAGE_TABLE_ENTRIES; i++)
	{
		_ppPt[i] = &_pPt[i];
	}

	// Identity map range 2Mib to 511MiB. fisrt 2MiB done in bootloader.
	// TODO: remove magic numbers.
	for (uint64_t i = 1; i < MAX_PAGE_TABLE_ENTRIES; i++)
	{
		for (uint64_t j = 0; j < MAX_PAGE_TABLE_ENTRIES; j++)
		{
			_ppPt[i]->value[j].value = 3 | ((0x200000 * (i + 1)) + (0x1000 * j));
		}
		_pPd->value[i].value = 3 | (uint64_t)_ppPt[i];
	}
}

void pageFaultHandler(registers_t* r)
{
	intptr_t addr;
	__asm__ volatile ("mov %%cr2, %0" : "=r" (addr));

	uint8_t present = !(r->err_code & 0x01);
	uint8_t rw = (r->err_code & 0x02) >> 1;
	uint8_t us = (r->err_code & 0x04) >> 2;
	uint8_t reserved = (r->err_code & 0x08) >> 3;
	uint8_t id = (r->err_code & 0x10) >> 4;
	UNUSED(id);

	char addrStr[512];
	kprint("Page fault!\n\t(");
	if (present) kprint("present");
	if (rw) kprint(" read-only");
	if (us) kprint(" user-mode");
	if (reserved) kprint(" reserved");
	kprint(") at 0x");
	kprint(itoa(addr, addrStr, 16));
	kprint(".\n");
}

