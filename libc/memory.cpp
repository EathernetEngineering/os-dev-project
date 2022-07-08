#include "libc/memory.hpp"
#include "libc/function.hpp"
#include "libc/stdlib.hpp"

#include "kernel/kprint.hpp"

#include "cpu/isr.hpp"

// Allocate memory statically for second through fourth page table.
PageTable _Pt2bss, _Pt3bss, _Pt4bss, _Pt5bss;

PageMapLevel4 *_Pml4;
PageDirectoryPointerTable *_Pdpt;
PageDirectory *_Pd;
PageTable *_ppPt[5];

void pageFaultHandler(registers_t* r);

void setupPaging()
{
	kprint("Setting ou paging.\n");

	registerInterruptHandler(14, pageFaultHandler);
	_Pml4 = (PageMapLevel4 *)(0x1000);
	_Pdpt = (PageDirectoryPointerTable *)(0x2000);
	_Pd = (PageDirectory *)(0x3000);

	_ppPt[0] = (PageTable *)(0x4000);
	_ppPt[1] = &_Pt2bss;
	_ppPt[2] = &_Pt3bss;
	_ppPt[3] = &_Pt4bss;
	_ppPt[4] = &_Pt5bss;

	// Identity map range 2Mib to 16MiB. fisrt 2MiB done in bootloader.
	// TODO: remove magic numbers.
	for (uint64_t i = 1; i < 5; i++)
	{
		for (uint64_t j = 0; j < MAX_PAGE_TABLE_ENTRIES; j++)
		{
			_ppPt[i]->value[j].value = 3 | ((0x200000 * (i + 1)) + (0x1000 * j));
		}
	}

	// Add newly mapped page tables to page directory.
	// TODO: remove magic numbers.
	for (uint32_t i = 1; i < 5; i++)
	{
		_Pd->value[i].value = 3 | (uint64_t)_ppPt[i];
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

