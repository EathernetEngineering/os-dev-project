#ifndef __DRIVERS__PORTS__H__
#define __DRIVERS__PORTS_H__


//////////////////////////////////////////////////
//// PORTS ///////////////////////////////////////
//////////////////////////////////////////////////

uint8_t portByteIn(uint16_t port);
void portByteOut(uint16_t port, uint8_t data);
uint16_t portWordIn(uint16_t port);
void portWordOut(uint16_t port, uint16_t data);


//////////////////////////////////////////////////
//// 64 BIT EXCLUSIVE ////////////////////////////
//////////////////////////////////////////////////

enum cpuidRequests
{
	CPUID_GETVENDORSTRING,
	CPUID_GETFEATURES,
	CPUID_GETTLB,
	CPUID_GETSERIAL,

	CPUID_INTELEXTENDED=0x80000000,
	CPUID_INTELFEATURES,
	CPUID_INTELBRANDSTRING,
	CPUID_INTELBRANDSTRINGMORE,
	CPUID_INTELBRANDSTRINGEND,
};

enum {
	CPUID_FEAT_ECX_SSE3         = 1 << 0,
	CPUID_FEAT_ECX_PCLMUL       = 1 << 1,
	CPUID_FEAT_ECX_DTES64       = 1 << 2,
	CPUID_FEAT_ECX_MONITOR      = 1 << 3,
	CPUID_FEAT_ECX_DS_CPL       = 1 << 4,
	CPUID_FEAT_ECX_VMX          = 1 << 5,
	CPUID_FEAT_ECX_SMX          = 1 << 6,
	CPUID_FEAT_ECX_EST          = 1 << 7,
	CPUID_FEAT_ECX_TM2          = 1 << 8,
	CPUID_FEAT_ECX_SSSE3        = 1 << 9,
	CPUID_FEAT_ECX_CID          = 1 << 10,
	CPUID_FEAT_ECX_SDBG         = 1 << 11,
	CPUID_FEAT_ECX_FMA          = 1 << 12,
	CPUID_FEAT_ECX_CX16         = 1 << 13,
	CPUID_FEAT_ECX_XTPR         = 1 << 14,
	CPUID_FEAT_ECX_PDCM         = 1 << 15,
	CPUID_FEAT_ECX_PCID         = 1 << 17,
	CPUID_FEAT_ECX_DCA          = 1 << 18,
	CPUID_FEAT_ECX_SSE4_1       = 1 << 19,
	CPUID_FEAT_ECX_SSE4_2       = 1 << 20,
	CPUID_FEAT_ECX_X2APIC       = 1 << 21,
	CPUID_FEAT_ECX_MOVBE        = 1 << 22,
	CPUID_FEAT_ECX_POPCNT       = 1 << 23,
	CPUID_FEAT_ECX_TSC          = 1 << 24,
	CPUID_FEAT_ECX_AES          = 1 << 25,
	CPUID_FEAT_ECX_XSAVE        = 1 << 26,
	CPUID_FEAT_ECX_OSXSAVE      = 1 << 27,
	CPUID_FEAT_ECX_AVX          = 1 << 28,
	CPUID_FEAT_ECX_F16C         = 1 << 29,
	CPUID_FEAT_ECX_RDRAND       = 1 << 30,
	CPUID_FEAT_ECX_HYPERVISOR   = 1 << 31,

	CPUID_FEAT_EDX_FPU          = 1 << 0,
	CPUID_FEAT_EDX_VME          = 1 << 1,
	CPUID_FEAT_EDX_DE           = 1 << 2,
	CPUID_FEAT_EDX_PSE          = 1 << 3,
	CPUID_FEAT_EDX_TSC          = 1 << 4,
	CPUID_FEAT_EDX_MSR          = 1 << 5,
	CPUID_FEAT_EDX_PAE          = 1 << 6,
	CPUID_FEAT_EDX_MCE          = 1 << 7,
	CPUID_FEAT_EDX_CX8          = 1 << 8,
	CPUID_FEAT_EDX_APIC         = 1 << 9,
	CPUID_FEAT_EDX_SEP          = 1 << 11,
	CPUID_FEAT_EDX_MTRR         = 1 << 12,
	CPUID_FEAT_EDX_PGE          = 1 << 13,
	CPUID_FEAT_EDX_MCA          = 1 << 14,
	CPUID_FEAT_EDX_CMOV         = 1 << 15,
	CPUID_FEAT_EDX_PAT          = 1 << 16,
	CPUID_FEAT_EDX_PSE36        = 1 << 17,
	CPUID_FEAT_EDX_PSN          = 1 << 18,
	CPUID_FEAT_EDX_CLFLUSH      = 1 << 19,
	CPUID_FEAT_EDX_DS           = 1 << 21,
	CPUID_FEAT_EDX_ACPI         = 1 << 22,
	CPUID_FEAT_EDX_MMX          = 1 << 23,
	CPUID_FEAT_EDX_FXSR         = 1 << 24,
	CPUID_FEAT_EDX_SSE          = 1 << 25,
	CPUID_FEAT_EDX_SSE2         = 1 << 26,
	CPUID_FEAT_EDX_SS           = 1 << 27,
	CPUID_FEAT_EDX_HTT          = 1 << 28,
	CPUID_FEAT_EDX_TM           = 1 << 29,
	CPUID_FEAT_EDX_IA64         = 1 << 30,
	CPUID_FEAT_EDX_PBE          = 1 << 31
};

static inline void cpuid(int code, uint32_t *a, uint32_t *d)
{
	__asm__ volatile ("cpuid":"=a"(*a), "=d"(*d):"a"(code):"ecx","ebx");
}

static inline void cpuGetMsr(uint32_t msr, uint32_t *lo, uint32_t *hi)
{
	__asm__ volatile ("rdmsr":"=a"(*lo), "=d"(*hi):"c"(msr));
}

static inline void cpuSetMsr(uint32_t msr, uint32_t lo, uint32_t hi)
{
	__asm__ volatile ("wrmsr"::"a"(lo), "d"(hi), "c"(msr));
}

bool checkApic();

uintptr_t cpuGetApicBase();
void cpuSetApicBase(uintptr_t apic);

enum ApicRegisters
{
	APIC_REGISTER_LOCAL_APIC_ID							= 0x0020,
	APIC_REGISTER_LOCAL_APIC_VERSION					= 0x0030,
	APIC_REGISTER_TASK_PRIORITY							= 0x0080,
	APIC_REGISTER_ARBITRATION_PRIORITY					= 0x0090,
	APIC_REGISTER_PROCESSOR_PRIORITY					= 0x00A0,
	APIC_REGISTER_EOI									= 0x00B0,
	APIC_REGISTER_REMOTE_READ							= 0x00C0,
	APIC_REGISTER_LOCAL_DESTINATION						= 0x00D0,
	APIC_REGISTER_DESTINATION_FORMAT					= 0x00E0,
	APIC_REGISTER_SPURIOUS_INTERRUPT_VECTOR				= 0x00F0,
	APIC_REGISTER_IN_SERVICE_1							= 0x0100,
	APIC_REGISTER_IN_SERVICE_2							= 0x0110,
	APIC_REGISTER_IN_SERVICE_3							= 0x0120,
	APIC_REGISTER_IN_SERVICE_4							= 0x0130,
	APIC_REGISTER_IN_SERVICE_5							= 0x0140,
	APIC_REGISTER_IN_SERVICE_6							= 0x0150,
	APIC_REGISTER_IN_SERVICE_7							= 0x0160,
	APIC_REGISTER_IN_SERVICE_8							= 0x0170,
	APIC_REGISTER_TRIGGER_MODE_1						= 0x0180,
	APIC_REGISTER_TRIGGER_MODE_2						= 0x0190,
	APIC_REGISTER_TRIGGER_MODE_3						= 0x01A0,
	APIC_REGISTER_TRIGGER_MODE_4						= 0x01B0,
	APIC_REGISTER_TRIGGER_MODE_5						= 0x01C0,
	APIC_REGISTER_TRIGGER_MODE_6						= 0x01D0,
	APIC_REGISTER_TRIGGER_MODE_7						= 0x01E0,
	APIC_REGISTER_TRIGGER_MODE_8						= 0x01F0,
	APIC_REGISTER_INTERRUPT_REQUEST_1					= 0x0200,
	APIC_REGISTER_INTERRUPT_REQUEST_2					= 0x0210,
	APIC_REGISTER_INTERRUPT_REQUEST_3					= 0x0220,
	APIC_REGISTER_INTERRUPT_REQUEST_4					= 0x0230,
	APIC_REGISTER_INTERRUPT_REQUEST_5					= 0x0240,
	APIC_REGISTER_INTERRUPT_REQUEST_6					= 0x0250,
	APIC_REGISTER_INTERRUPT_REQUEST_7					= 0x0260,
	APIC_REGISTER_INTERRUPT_REQUEST_8					= 0x0270,
	APIC_REGISTER_ERROR_STATUS							= 0x0280,
	APIC_REGISTER_LVT_CMCI								= 0x02F0,
	APIC_REGISTER_INTERRUPT_COMMAND_1					= 0x0300,
	APIC_REGISTER_INTERRUPT_COMMAND_2					= 0x0310,
	APIC_REGISTER_LVT_TIMER								= 0x0320,
	APIC_REGISTER_LVT_THERMAL_SENSOR					= 0x0330,
	APIC_REGISTER_LVT_PERFORMANCE_MONITORING_COUNTERS	= 0x0340,
	APIC_REGISTER_LVT_LINT0								= 0x0350,
	APIC_REGISTER_LVT_LINT1								= 0x0360,
	APIC_REGISTER_LVT_ERROR								= 0x0370,
	APIC_REGISTER_INITAL_COUNT							= 0x0380,
	APIC_REGISTER_CURRENT_COUNT							= 0x0390,
	APIC_REGISTER_DIVIDE_CONFIGURATION					= 0x03E0
};

enum ApicLvtFlags
{
	APIC_LVT_DELIVERY_MODE_SMI = 0x200,
	APIC_LVT_DELIVERY_MODE_NMI = 0x400,
	APIC_LVT_DELIVERY_MODE_EXTINT = 0x700,
	APIC_LVT_DELIVERY_MODE_INIT = 0x500,
	APIC_LVT_DELIVERY_STATUS_SEND_PENDING = 0x1000,
	APIC_LVT_INTERRUPT_INPUT_PIN_POLARITY = 0x2000,
	APIC_LVT_REMOTE_IRR = 0x4000,
	APIC_LVT_MASK = 0x10000,
	APIC_LVT_TIMER_MODE_PERIODIC = 0x20000,
	APIC_LVT_TIMER_MODE_TSC_DEADLINE = 0x40000
};
uint32_t readApicRegister(uint32_t regOffset);
void writeApicRegister(uint32_t regOffset, uint32_t value);

#endif

