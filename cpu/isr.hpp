#ifndef __CPU__ISR_H__
#define __CPU__ISR_H__

extern "C" {
	extern void __attribute__((naked)) isr0();
	extern void __attribute__((naked)) isr1();
	extern void __attribute__((naked)) isr2();
	extern void __attribute__((naked)) isr3();
	extern void __attribute__((naked)) isr4();
	extern void __attribute__((naked)) isr5();
	extern void __attribute__((naked)) isr6();
	extern void __attribute__((naked)) isr7();
	extern void __attribute__((naked)) isr8();
	extern void __attribute__((naked)) isr9();
	extern void __attribute__((naked)) isr10();
	extern void __attribute__((naked)) isr11();
	extern void __attribute__((naked)) isr12();
	extern void __attribute__((naked)) isr13();
	extern void __attribute__((naked)) isr14();
	extern void __attribute__((naked)) isr15();
	extern void __attribute__((naked)) isr16();
	extern void __attribute__((naked)) isr17();
	extern void __attribute__((naked)) isr18();
	extern void __attribute__((naked)) isr19();
	extern void __attribute__((naked)) isr20();
	extern void __attribute__((naked)) isr21();
	extern void __attribute__((naked)) isr22();
	extern void __attribute__((naked)) isr23();
	extern void __attribute__((naked)) isr24();
	extern void __attribute__((naked)) isr25();
	extern void __attribute__((naked)) isr26();
	extern void __attribute__((naked)) isr27();
	extern void __attribute__((naked)) isr28();
	extern void __attribute__((naked)) isr29();
	extern void __attribute__((naked)) isr30();
	extern void __attribute__((naked)) isr31();

	extern void __attribute__((naked)) irq0();
	extern void __attribute__((naked)) irq1();
	extern void __attribute__((naked)) irq2();
	extern void __attribute__((naked)) irq3();
	extern void __attribute__((naked)) irq4();
	extern void __attribute__((naked)) irq5();
	extern void __attribute__((naked)) irq6();
	extern void __attribute__((naked)) irq7();
	extern void __attribute__((naked)) irq8();
	extern void __attribute__((naked)) irq9();
	extern void __attribute__((naked)) irq10();
	extern void __attribute__((naked)) irq11();
	extern void __attribute__((naked)) irq12();
	extern void __attribute__((naked)) irq13();
	extern void __attribute__((naked)) irq14();
	extern void __attribute__((naked)) irq15();
	extern void __attribute__((naked)) svr();
}

#define PORT_PIC_MASTER_COMMAND     0x20
#define PORT_PIC_MASTER_DATA        0x21
#define PORT_PIC_SLAVE_COMMAND      0xA0
#define PORT_PIC_SLAVE_DATA         0xA1

enum ICW1
{
	ICW1_icw4           = 0b00000001,
	ICW1_single         = 0b00000010,
	ICW1_addrInterval   = 0b00000100,
	ICW1_triggerMode    = 0b00001000,
	ICW1_set            = 0b00010000
};

enum ICW4
{
	ICW4_8086           = 0b00000001,
	ICW4_autoEoi        = 0b00000010,
	ICW4_bufMorS        = 0b00000100,
	ICW4_buf            = 0b00001000,
	ICW4_sfnm           = 0b00010000
};

#define IRQ0     32
#define IRQ1     33
#define IRQ2     34
#define IRQ3     35
#define IRQ4     36
#define IRQ5     37
#define IRQ6     38
#define IRQ7     39
#define IRQ8     40
#define IRQ9     41
#define IRQ10    42
#define IRQ11    43
#define IRQ12    44
#define IRQ13    45
#define IRQ14    46
#define IRQ15    47
#define svr      0xFF

namespace exceptions
{
	enum cpuExceptions
	{
		divisionByZeroException             =  0,
		debugException                      =  1,
		nonMaskableInterruptException       =  2,
		breakpointException                 =  3,
		overflowException                   =  4,
		boundRangeExceededException         =  5,
		invalidOpcodeException              =  6,
		deviceNotAvailableException         =  7,
		doubleFaultException                =  8,
		coprocessorSegmentOverrunException  =  9,
		invalidTssException                 = 10,
		segmentNotPresentException          = 11,
		stackSegmentFaultException          = 12,
		generalProtectionFaultException     = 13,
		pageFaultException                  = 14,
		x87FloatingPointException           = 16,
		alignmentCheckException             = 17,
		machineCheckException               = 18,
		simdFloatingPointException          = 19,
		virtualizationException             = 20,
		controlProtectionException          = 21,
		hypervisorInjectionException        = 28,
		vmmCommunicationException           = 29,
		securityException                   = 30,
	};
}

typedef struct {
	uint64_t ds;
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
	uint64_t rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
	uint64_t int_no, err_code;
	uint64_t rip, cs, eflags, userrsp, ss;
} __attribute__((__packed__)) registers_t;

void isrInstall();

extern "C" void isrHandler(registers_t *r);
extern "C" void ireqHandler(registers_t* r);

void irqInstall();

typedef void (*isr_t)(registers_t *);
void registerInterruptHandler(uint8_t n, isr_t handler);

#endif

