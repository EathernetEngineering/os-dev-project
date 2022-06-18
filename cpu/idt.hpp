#ifndef __CPU__IDT_H__
#define __CPU__IDT_H__

#define KERNEL_CS 0x08

	typedef struct {
		uint16_t offsetLow;
		uint16_t selector;
		uint8_t ist;
		uint8_t typeAttribs;
		uint16_t offsetMid;
		uint32_t offsetHigh;
		uint32_t reserved;
	} __attribute__((packed)) idt_entry_t ;

	typedef struct {
		uint16_t limit;
		uint64_t base;
	} __attribute__((packed)) idt_ptr_t;

#define IDT_ENTRIES 256
	extern idt_entry_t idt[IDT_ENTRIES];
	extern idt_ptr_t idtRegister;

	void zeroIdt();
	void setIdtGate(uint32_t n, uint64_t handler);
	void setIdt();

#endif

