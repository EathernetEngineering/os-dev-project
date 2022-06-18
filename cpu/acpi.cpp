#include "cpu/acpi.hpp"

#include "drivers/screen.hpp"

#include "libc/memory.hpp"
#include "libc/string.hpp"

static AcpiSdtHeader *_RootSdt;

uintptr_t GetRsdtBaseAddress()
{
	return (uintptr_t)_RootSdt;
}

void initAcpi()
{
	kprint("Initalizing kernel ACPI features.\n");
	// Find the rsdp in memory.

	RsdpDescriptor *descriptor = NULL;

	for (uintptr_t location = 0x000E0000; location < 0x000FFFFF; location += 16)
	{
		if (memcmp("RSD PTR ", (char*)location, 8) == 0)
		{
			descriptor = (RsdpDescriptor *)(void *)location;
			break;
		}
	}

	if (descriptor == NULL)
	{
		kprint("Failed to find rsd\n");
		return;
	}

	// Checksum.

	int32_t sum = 0;
	for (uint32_t i = 0; i < sizeof(RsdpDescriptor); i++)
	{
		sum += *(((uint8_t *)descriptor) + i);
	}
	if ((sum & 0xFF) != 0)
	{
		kprint("Checksum Failed\n");
		return;
	}

	// use version 1.0 or 2.0+

	if (descriptor->revision == 2)
	{
		// Checksum again

		sum = 0;
		for (uint32_t i = 0; i < sizeof(RsdpDescriptor2); i++)
		{
			sum += *(((uint8_t*)descriptor) + i);
		}
		if ((sum & 0xFF) != 0)
		{
			kprint("Checksum 2.0(+) Failed\n");
			return;
		}
		_RootSdt = (AcpiSdtHeader *)(((RsdpDescriptor2 *)descriptor)->xsdtAddress);
	}
	else
	{
		_RootSdt = (AcpiSdtHeader *)((uintptr_t)descriptor->rsdtAddress);
	}
}

uintptr_t getAcpiSdt(const char *signature)
{
	size_t numEntries = (_RootSdt->length - sizeof(AcpiSdtHeader)) / 4;
	uint32_t *entries = (uint32_t*)((uintptr_t)(_RootSdt) + sizeof(AcpiSdtHeader));

	for (uint32_t i = 0; i < numEntries; i++)
	{
		if (memcmp(signature, &entries[i], 4) == 0)
		{
			return (uintptr_t)entries[i];
		}
	}
	return (uintptr_t)-1;
}

