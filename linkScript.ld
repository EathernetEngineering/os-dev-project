ENTRY(kentry)
OUTPUT_FORMAT(elf64-x86-64)

STARTUP(obj/kernel_entry.o)
ENTRY(_start)

SECTIONS
{
	. = 0x1M;

	.text : ALIGN(4K) {
		*(.text)
	}

	.rodata : ALIGN(4K) {
		*(.rodata)
	}

	.data : ALIGN(4K) {
		*(.data)
	}

	.bss : ALIGN(4K) {
		*(COMMON)
		*(.bss)
	}
}

