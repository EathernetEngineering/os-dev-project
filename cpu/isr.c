#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "ports.h"
#include "../kernel/util.h"
#include "../libc/string.h"

isr_t interrupt_handlers[256];

void isr_install() {
	set_idt_gate(0,  (unsigned)isr0);
	set_idt_gate(1,  (unsigned)isr1);
	set_idt_gate(2,  (unsigned)isr2);
	set_idt_gate(3,  (unsigned)isr3);
	set_idt_gate(4,  (unsigned)isr4);
	set_idt_gate(5,  (unsigned)isr5);
	set_idt_gate(6,  (unsigned)isr6);
	set_idt_gate(7,  (unsigned)isr7);
	set_idt_gate(8,  (unsigned)isr8);
	set_idt_gate(9,  (unsigned)isr9);
	set_idt_gate(10, (unsigned)isr10);
	set_idt_gate(11, (unsigned)isr11);
	set_idt_gate(12, (unsigned)isr12);
	set_idt_gate(13, (unsigned)isr13);
	set_idt_gate(14, (unsigned)isr14);
	set_idt_gate(15, (unsigned)isr15);
	set_idt_gate(16, (unsigned)isr16);
	set_idt_gate(17, (unsigned)isr17);
	set_idt_gate(18, (unsigned)isr18);
	set_idt_gate(19, (unsigned)isr19);
	set_idt_gate(20, (unsigned)isr20);
	set_idt_gate(21, (unsigned)isr21);
	set_idt_gate(22, (unsigned)isr22);
	set_idt_gate(23, (unsigned)isr23);
	set_idt_gate(24, (unsigned)isr24);
	set_idt_gate(25, (unsigned)isr25);
	set_idt_gate(26, (unsigned)isr26);
	set_idt_gate(27, (unsigned)isr27);
	set_idt_gate(28, (unsigned)isr28);
	set_idt_gate(29, (unsigned)isr29);
	set_idt_gate(30, (unsigned)isr30);
	set_idt_gate(31, (unsigned)isr31);

	port_byte_out(0x20, 0x11);
	port_byte_out(0xA0, 0x11);
	port_byte_out(0x21, 0x20);
	port_byte_out(0xA1, 0x28);
	port_byte_out(0x21, 0x04);
	port_byte_out(0xA1, 0x02);
	port_byte_out(0x21, 0x01);
	port_byte_out(0xA1, 0x01);
	port_byte_out(0x21, 0x00);
	port_byte_out(0xA1, 0x00);

	set_idt_gate(32, (unsigned)irq0);
	set_idt_gate(33, (unsigned)irq1);
	set_idt_gate(34, (unsigned)irq2);
	set_idt_gate(35, (unsigned)irq3);
	set_idt_gate(36, (unsigned)irq4);
	set_idt_gate(37, (unsigned)irq5);
	set_idt_gate(38, (unsigned)irq6);
	set_idt_gate(39, (unsigned)irq7);
	set_idt_gate(40, (unsigned)irq8);
	set_idt_gate(41, (unsigned)irq9);
	set_idt_gate(42, (unsigned)irq10);
	set_idt_gate(43, (unsigned)irq11);
	set_idt_gate(44, (unsigned)irq12);
	set_idt_gate(45, (unsigned)irq13);
	set_idt_gate(46, (unsigned)irq14);
	set_idt_gate(47, (unsigned)irq15);

	set_idt();
}

char *exception_messages[] = {
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
	"Unknown Interrupt",

	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",

	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

void isr_handler(registers_t r)
{
	kprint("received interrupt: ");
	char s[3];
	int_to_ascii(r.int_no, s);
	kprint(s);
	kprint("\n");
	kprint(exception_messages[r.int_no]);
	kprint("\n");
}

void register_interrupt_handler(unsigned char n, isr_t handler)
{
	interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r)
{
	if (r.int_no >= 40) port_byte_out(0x0A, 0x20);
	port_byte_out(0x20, 0x20);

	if (interrupt_handlers[r.int_no] != 0)
	{
		isr_t handler = interrupt_handlers[r.int_no];
		handler(r);
	}
}

