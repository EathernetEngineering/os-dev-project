#include "cpu/page.hpp"
#include "cpu/isr.hpp"

#include "klibc/function.hpp"
#include "klibc/stdlib.hpp"
#include "klibc/kprint.hpp"

/* BIOS SET */
PageMapLevel4 *_pPml4;
PageDirectoryPointerTable *_pPdpt;
PageDirectory *_pPd;
PageTable *_ppPt[11];
/* END BIOS SET */

PageMapLevel4 *_pPageMap;

void pageFaultHandler(registers_t* r);

void setupPaging()
{
	kprint("Setting ou paging.\n");

	registerInterruptHandler(14, pageFaultHandler);
	_pPml4 = reinterpret_cast<PageMapLevel4 *>(0x1000);
	_pPdpt = reinterpret_cast<PageDirectoryPointerTable *>(0x2000);
	_pPd = reinterpret_cast<PageDirectory *>(0x3000);

	// Map tables set by bootloader to kernel varables
	for (uint32_t i = 1; i < 11; i++)
	{
		_ppPt[0] = reinterpret_cast<PageTable *>(0x4000 + 0x1000 * i);
	}

	// reset page table location to 7MiB, map 4GiB
	_pPageMap = reinterpret_cast<PageMapLevel4 *>(0x700000);
	ZeroMemory(_pPageMap, 0x900000);
	for (uint64_t i = 0; i < 4; i++)
	{
		for (uint64_t j = 0; j < MAX_PAGE_TABLE_ENTRIES; j++)
		{
			for (uint64_t k = 0; k < MAX_PAGE_TABLE_ENTRIES; k++)
			{
				uintptr_t pteAddress =
					0x800000ll + (0x200000 * i) +
					(0x1000ll * j) + (0x08ll * k);
				reinterpret_cast<Pte *>(pteAddress)->value =
					3ll | (0x1000ll * (i  +j + k));
			}
			uintptr_t pdeAddress =
				0x702000ll + (0x1000ll * i) + (0x08ll * j);
			reinterpret_cast<Pde *>(pdeAddress)->value =
				3ll | (0x800000ll + (0x1000ll * (i + j)));
		}
		uintptr_t pdptAddress =
			0x701000ll + (0x08ll * i);
		reinterpret_cast<Pdpte *>(pdptAddress)->value =
			3ll | (0x702000ll + (0x1000ll * i));
	}
	_pPageMap->entries[0].value =
		3ll | 0x701000ll;

	__asm__ volatile("movq %0, %%cr3" : : "r"(_pPageMap));
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

