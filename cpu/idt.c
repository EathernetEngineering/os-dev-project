#include "idt.h"
#include "../kernel/util.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int n, unsigned handler)
{
	idt[n].low_offset = LOWORD(handler);
	idt[n].sel = KERNEL_CS;
	idt[n].always0 = 0;
	idt[n].flags = 0x8E;
	idt[n].high_offset = HIWORD(handler);
}

void set_idt()
{
	idt_reg.base = (unsigned)&idt;
	idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
	__asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}

