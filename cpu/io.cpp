#include "cpu/io.hpp"

#define IA32_APIC_BASE_MSR 0x1B
#define IA32_APIC_BASE_MSR_BSP 0x100
#define IA32_APIC_BASE_MSR_ENABLE 0x800

uintptr_t _ApicRegisterBase = 0x00;

uint8_t portByteIn(uint16_t port)
{
	unsigned char result;
	__asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
	return result;
}

void portByteOut(uint16_t port, uint8_t data)
{
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t portWordIn(uint16_t port)
{
	unsigned char result;
	__asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
	return result;
}

void portWordOut(uint16_t port, uint16_t data)
{
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

bool checkApic()
{
	uint32_t eax, edx;
	cpuid(CPUID_GETFEATURES, &eax, &edx);
	return edx & CPUID_FEAT_EDX_APIC;
}

uintptr_t cpuGetApicBase()
{
	uint32_t eax, edx;
	cpuGetMsr(IA32_APIC_BASE_MSR, &eax, &edx);

	uintptr_t madtSdtBase = -1;//getAcpiSdt("MADT");
	if (madtSdtBase == (uintptr_t)-1)
	{
#if defined(__PHYSICAL_ADDRESS_EXTENSION__)
		return (_ApicRegisterBase = (eax & 0xFFFFF000) | ((uint64_t)(edx & 0x0F) << 32));
#else
		return (_ApicRegisterBase = (eax & 0xFFFFF000));
#endif
	}

	return (_ApicRegisterBase = *(uint32_t*)(madtSdtBase + 0x24));
}

void cpuSetApicBase(uintptr_t apic)
{
	uint32_t edx = 0, eax = (apic & 0xFFFFF0000) | IA32_APIC_BASE_MSR_ENABLE;

#if defined(__PHYSICAL_ADDRESS_EXTENSION__)
	edx = (apic >> 32) & 0x0F;
#endif

	cpuSetMsr(IA32_APIC_BASE_MSR, eax, edx);
	_ApicRegisterBase = cpuGetApicBase();
}

uint32_t readApicRegister(uint32_t regOffset)
{
	if (_ApicRegisterBase == (uintptr_t)NULL) cpuGetApicBase();
	return *(uint32_t volatile*)((uintptr_t)_ApicRegisterBase + regOffset);
}

void writeApicRegister(uint32_t regOffset, uint32_t data)
{

	if (_ApicRegisterBase == (uintptr_t)NULL) cpuGetApicBase();
	*(uint32_t volatile*)((uintptr_t)_ApicRegisterBase + regOffset) = data;
}

