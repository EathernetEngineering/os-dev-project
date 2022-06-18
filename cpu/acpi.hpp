#ifndef _ACPI_H
#define _ACPI_H

typedef struct
{
	char signature[8];
	uint8_t checksum;
	char oemId[6];
	uint8_t revision;
	uint32_t rsdtAddress;
} __attribute__((__packed__)) RsdpDescriptor;

typedef struct
{
	RsdpDescriptor descriptor1;
	uint32_t length;
	uint64_t xsdtAddress;
	uint8_t extendedChecksum;
	uint8_t reserved[3];
} __attribute__((__packed__)) RsdpDescriptor2;

typedef struct
{
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oemId[6];
	char oemTableId[8];
	uint32_t oemRevision;
	uint32_t CreatorId;
	uint32_t creatorRevision;
} __attribute__((__packed__)) AcpiSdtHeader;

void initAcpi();

uintptr_t getAcpiSdt(const char *signature);

#endif

