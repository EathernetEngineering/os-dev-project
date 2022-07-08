#include "kernel/disk.hpp"

#include "cpu/io.hpp"

#include "libc/function.hpp"

#define DRIVE_DATA_PORT             0x1F0
#define DRIVE_SECTOR_COUNT_PORT     0x1F2
#define DRIVE_LBA_LOW_PORT          0x1F3
#define DRIVE_SECTOR_NUMBER_PORT    0x1F3
#define DRIVE_LBA_MID_PORT          0x1F4
#define DRIVE_CYLINDER_LOW_PORT     0x1F4
#define DRIVE_LBA_HIGH_PORT         0x1F5
#define DRIVE_CYLINDER_HIGH_PORT    0x1F5
#define DRIVE_HEAD_PORT             0x1F6
#define DRIVE_COMMAND_PORT          0x1F7

#define DRIVE_COMMAND_READ          0x20
#define DRIVE_COMMAND_WRITE         0x30

void *readSectrors(uint16_t driveNumber,
		lba_address_t offset,
		uint32_t sectors,
		void *buffer)
{
	UNUSED(driveNumber);
	// Send count of sectors to read
	portByteOut(DRIVE_SECTOR_COUNT_PORT, sectors);

	// Send low byte of LBA
	portByteOut(DRIVE_LBA_LOW_PORT, (offset & 0xFF));

	// Send middle byte of LBA
	portByteOut(DRIVE_LBA_MID_PORT, (offset & 0xFF00) >> 8);

	// Send high byte of LBA
	portByteOut(DRIVE_LBA_HIGH_PORT, (offset & 0xFF0000) >> 16);

	// Send LBA read flag, drive number and last 4 bits of LBA
	portByteOut(DRIVE_HEAD_PORT, ((offset & 0xF000000) >> 24) | 0b11100000);

	// Send command to read
	portByteOut(DRIVE_COMMAND_PORT, DRIVE_COMMAND_READ);

	// Wait until sector buffer is ready
	while (!(portByteIn(DRIVE_COMMAND_PORT) & 8));

	// Copy buffer from data port to buffer
	for (size_t i = 0; i < 256 * sectors - 1; i++)
	{
		reinterpret_cast<uint16_t *>(buffer)[i] = portWordIn(DRIVE_DATA_PORT);
	}

	// return pointer to the buffer provided
	return buffer;
}

void *readSectrors(uint16_t driveNumber,
		chs_address_t offset,
		uint32_t sectors,
		void *buffer)
{
	UNUSED(driveNumber);
	// Send count of sectors to read
	portByteOut(DRIVE_SECTOR_COUNT_PORT, sectors);

	// Send sector number
	portByteOut(DRIVE_SECTOR_NUMBER_PORT, offset.sector);

	// Send low byte of cylinder
	portByteOut(DRIVE_CYLINDER_LOW_PORT, offset.cylinder & 0xFF);

	// Send high byte of cylinder
	portByteOut(DRIVE_CYLINDER_HIGH_PORT, (offset.cylinder & 0xFF00) >> 8);

	// Send drive number and head
	portByteOut(DRIVE_HEAD_PORT, (offset.head & 0b1111) | 0b10100000);

	// Send command to read
	portByteOut(DRIVE_COMMAND_PORT, DRIVE_COMMAND_READ);

	// Wait until sector buffer is ready
	while (!(portByteIn(DRIVE_COMMAND_PORT) & 8));

	// Copy buffer from data port to buffer
	for (size_t i = 0; i < 256 * sectors - 1; i++)
	{
		reinterpret_cast<uint16_t *>(buffer)[i] = portWordIn(DRIVE_DATA_PORT);
	}

	// return pointer to the buffer provided
	return buffer;
}

void *writeSectrors(uint16_t driveNumber,
		lba_address_t offset,
		uint32_t sectors,
		void *buffer)
{
	UNUSED(driveNumber);
	UNUSED(offset);
	UNUSED(sectors);
	return buffer;
}

void *writeSectrors(uint16_t driveNumber,
		chs_address_t offset,
		uint32_t sectors,
		void *buffer)
{
	UNUSED(driveNumber);
	UNUSED(offset);
	UNUSED(sectors);
	return buffer;
}

