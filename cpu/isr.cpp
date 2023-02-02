#include "cpu/isr.hpp"
#include "cpu/idt.hpp"
#include "cpu/io.hpp"

#include "klibc/memory.hpp"
#include "klibc/stdlib.hpp"
#include "klibc/string.hpp"
#include "klibc/function.hpp"

#include "drivers/screen.hpp"

#include <cpuid.h>

///////////////////////////////////////////////////
//// LOCAL VARIABLES //////////////////////////////
///////////////////////////////////////////////////

bool _ApicEnabled = false;
isr_t _InterruptHandlers[256];

uint16_t _IrqMasks = 0x00;

///////////////////////////////////////////////////
//// FUNCTION FORWARD DECLARATIONS ////////////////
///////////////////////////////////////////////////

void initalizePic();
void enablePic();
void disablePic();

void initalizeApic();

void irqSetMask(uint8_t irqLine, bool enabled);
void irqSendEoiPic(registers_t *r);
void irqSendEoiApic(registers_t *);

///////////////////////////////////////////////////
//// FUNCTION DEFINITIONS /////////////////////////
///////////////////////////////////////////////////

void isrInstall()
{
	kprint("Initalizing interrupts.\n");
	zeroIdt();
	ZeroMemory(_InterruptHandlers, sizeof(_InterruptHandlers));

	setIdtGate(0,  (uint64_t)isr0);
	setIdtGate(1,  (uint64_t)isr1);
	setIdtGate(2,  (uint64_t)isr2);
	setIdtGate(3,  (uint64_t)isr3);
	setIdtGate(4,  (uint64_t)isr4);
	setIdtGate(5,  (uint64_t)isr5);
	setIdtGate(6,  (uint64_t)isr6);
	setIdtGate(7,  (uint64_t)isr7);
	setIdtGate(8,  (uint64_t)isr8);
	setIdtGate(9,  (uint64_t)isr9);
	setIdtGate(10, (uint64_t)isr10);
	setIdtGate(11, (uint64_t)isr11);
	setIdtGate(12, (uint64_t)isr12);
	setIdtGate(13, (uint64_t)isr13);
	setIdtGate(14, (uint64_t)isr14);
	setIdtGate(15, (uint64_t)isr15);
	setIdtGate(16, (uint64_t)isr16);
	setIdtGate(17, (uint64_t)isr17);
	setIdtGate(18, (uint64_t)isr18);
	setIdtGate(19, (uint64_t)isr19);
	setIdtGate(20, (uint64_t)isr20);
	setIdtGate(21, (uint64_t)isr21);
	setIdtGate(22, (uint64_t)isr22);
	setIdtGate(23, (uint64_t)isr23);
	setIdtGate(24, (uint64_t)isr24);
	setIdtGate(25, (uint64_t)isr25);
	setIdtGate(26, (uint64_t)isr26);
	setIdtGate(27, (uint64_t)isr27);
	setIdtGate(28, (uint64_t)isr28);
	setIdtGate(29, (uint64_t)isr29);
	setIdtGate(30, (uint64_t)isr30);
	setIdtGate(31, (uint64_t)isr31);

	setIdtGate(32, (uint64_t)irq0);
	setIdtGate(33, (uint64_t)irq1);
	setIdtGate(34, (uint64_t)irq2);
	setIdtGate(35, (uint64_t)irq3);
	setIdtGate(36, (uint64_t)irq4);
	setIdtGate(37, (uint64_t)irq5);
	setIdtGate(38, (uint64_t)irq6);
	setIdtGate(39, (uint64_t)irq7);
	setIdtGate(40, (uint64_t)irq8);
	setIdtGate(41, (uint64_t)irq9);
	setIdtGate(42, (uint64_t)irq10);
	setIdtGate(43, (uint64_t)irq11);
	setIdtGate(44, (uint64_t)irq12);
	setIdtGate(45, (uint64_t)irq13);
	setIdtGate(46, (uint64_t)irq14);
	setIdtGate(47, (uint64_t)irq15);
	setIdtGate(0xFF, (uint64_t)svr);

	setIdt();

	initalizePic();
	if (checkApic())
	{
		disablePic();
		initalizeApic();
	}
}

const char *exceptionMessages[] = {
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",

	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Reserved",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"SIMD Floating-Point Exception",
	"Virtualization Exception",
	"Conttrol Protection Exception",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Hypervisor Injection Exception",
	"VMM Communication Exception",
	"Security Exception",
	"Reserved"
};

extern "C" void isrHandler(registers_t *r)
{
	kprint("received interrupt: ");
	char s[256];
	memset(s, 0, 256);
	int_to_ascii(r->int_no, s);
	kprint(s);
	kprint("\n\t");
	kprint(exceptionMessages[r->int_no]);
	if (r->err_code)
	{
		kprint("\n\tError code: ");
		kprint(itoa(r->err_code, s, 2));
	}
	kprint("\n");
	if (_InterruptHandlers[r->int_no] != 0)
	{
		isr_t handler = _InterruptHandlers[r->int_no];
		handler(r);
	}
}

void registerInterruptHandler(unsigned char n, isr_t handler)
{
	_InterruptHandlers[n] = handler;
}

extern "C" void irqHandler(registers_t *r)
{
	if (_InterruptHandlers[r->int_no] != 0)
	{
		isr_t handler = _InterruptHandlers[r->int_no];
		handler(r);
	}

	if (_ApicEnabled) irqSendEoiApic(r);
	else irqSendEoiPic(r);
}

void irqInstall()
{
	kprint("Initalizing IRQ's.\n");
	asm volatile("sti");
//	initTimer(50);
//	init_keyboard();
}

void initalizePic()
{
	portByteOut(PORT_PIC_MASTER_COMMAND, ICW1_icw4 | ICW1_single); // ICW1: start initalization sequence
	portByteOut(PORT_PIC_SLAVE_COMMAND, ICW1_icw4 | ICW1_single); //ICW1: start initalization sequence
	portByteOut(PORT_PIC_MASTER_DATA, IRQ0); //ICW2: Master PIC offset
	portByteOut(PORT_PIC_SLAVE_DATA, IRQ8); // ICW2: Slave PIC offset
	portByteOut(PORT_PIC_MASTER_DATA, 0x04); // ICW3: tell master PIC there is a slave PIC at IRQ2
	portByteOut(PORT_PIC_SLAVE_DATA, 0x02); // ICW3: tell slave PIC its cascade identity
	portByteOut(PORT_PIC_MASTER_DATA, ICW4_8086); // ICW4: enable x86 mode
	portByteOut(PORT_PIC_SLAVE_DATA, ICW4_8086); // ICW4: enable x86 mode
	_IrqMasks = (uint16_t)0x00; // initalize masks to NULL
	portByteOut(PORT_PIC_MASTER_DATA, (_IrqMasks & 0x00FF)); // Set masks
	portByteOut(PORT_PIC_SLAVE_DATA, ((_IrqMasks & 0xFF00) >> 8)); // Set masks
	_ApicEnabled = false; // using 8259 PIC rather than APIC
}

void enablePic()
{
	portByteOut(PORT_PIC_MASTER_DATA, (uint8_t)(_IrqMasks & 0x00FF)); // Set masks
	portByteOut(PORT_PIC_SLAVE_DATA, (uint8_t)((_IrqMasks & 0xFF00) >> 8)); // Set masks
}

void disablePic()
{
	portByteOut(PORT_PIC_MASTER_DATA, 0xFF); // Set masks to 0xFF to disable PIC
	portByteOut(PORT_PIC_SLAVE_DATA, 0xFF); // Set masks to 0xFF to disable PIC
}

void initalizeApic()
{
	cpuSetApicBase(cpuGetApicBase());
	//writeApicRegister(APIC_REGISTER_SPURIOUS_INTERRUPT_VECTOR, readApicRegister(APIC_REGISTER_SPURIOUS_INTERRUPT_VECTOR) | 0x100 | 0xFF);
	_ApicEnabled = true;
}

void irqSetMask(uint8_t irqLine, bool enabled)
{
	if (irqLine < 15) return;
	_IrqMasks &= ~((uint16_t)enabled << irqLine);
	if (_ApicEnabled)
	{

	}
	else
	{
		if (irqLine < 8)
		{
			portByteOut(PORT_PIC_MASTER_DATA, (uint8_t)(_IrqMasks & 0xFF));
		}
		else
		{
			portByteOut(PORT_PIC_SLAVE_DATA, (uint8_t)((_IrqMasks & 0xFF00) >> 8));
		}
	}
}

void irqSendEoiApic(registers_t*)
{
	writeApicRegister(APIC_REGISTER_EOI, 0x00);
}

void irqSendEoiPic(registers_t *r)
{
	if (r->int_no >= 40) portByteOut(0x0A, 0x20);
	portByteOut(0x20, 0x20);
}

