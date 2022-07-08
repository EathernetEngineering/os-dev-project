#ifndef _KERNEL_DISK_HPP
#define _KERNEL_DISK_HPP

#include "stddef.h"

typedef uint32_t lba_address_t;

typedef struct
{
	uint16_t cylinder;
	uint16_t head : 10;
	uint16_t sector : 6;
} __attribute__((packed)) chs_address_t;

typedef struct
{
	uint16_t cylinders;
	uint16_t heads;
	uint16_t sectors;
} drive_layout_t;

void *readSectrors(uint16_t driveNumber,
		const lba_address_t &offset,
		uint32_t sectors,
		void *buffer);

void *readSectrors(uint16_t driveNumber,
		const chs_address_t &offset,
		uint32_t sectors,
		void *buffer);

void *writeSectrors(uint16_t driveNumber,
		const lba_address_t &offset,
		uint32_t sectors,
		void *buffer);

void *writeSectrors(uint16_t driveNumber,
		const chs_address_t &offset,
		uint32_t sectors,
		void *buffer);

inline lba_address_t translateChs(const chs_address_t &addr,
		const drive_layout_t &layout)
{
	return (addr.cylinder * (uint32_t)layout.heads + addr.head)
		* (uint32_t)layout.sectors
		+ (addr.sector - 1);
}

#endif

