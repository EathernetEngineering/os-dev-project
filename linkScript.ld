ENTRY(kentry)
OUTPUT_FORMAT(elf64-x86-64)

STARTUP(obj/kernel_entry.o)

OUTPUT(obj/kernel.elf)

SECTIONS
{
	.text 0x100000 : {
		*(.text)
	}

	.rodata : {
		*(.rodata)
	}

	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
		*(COMMON)
	}
}

