#include "cpu/idt.hpp"
#include "klibc/function.hpp"

idt_entry_t idt[IDT_ENTRIES];
idt_ptr_t idt_ptr;

void zeroIdt()
{
	ZeroMemory(idt, sizeof(idt_entry_t) * IDT_ENTRIES);
}

void setIdtGate(uint32_t n, uint64_t handler)
{
	idt[n].offsetLow = handler & 0xFFFF;
	idt[n].selector = KERNEL_CS;
	idt[n].ist = 0x00;
	idt[n].typeAttribs = 0x8E;
	idt[n].offsetMid = (handler >> 16) & 0xFFFF;
	idt[n].offsetHigh = (handler >> 32) & 0xFFFFFFFF;
	idt[n].reserved = 0x00;
}

void setIdt()
{
	idt_ptr.base = (uint64_t)&idt;
	idt_ptr.limit = IDT_ENTRIES * sizeof(idt_ptr_t) - 1;
	__asm__ volatile ("lidt (%0)" : : "r" (&idt_ptr));
}

