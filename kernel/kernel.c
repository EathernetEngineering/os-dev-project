#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/timer.h"
#include "../libc/string.h"

char* load_kernel_string;

void _start()
{
	kprint_at("Loaded Kernel!", 0, 1);
	isr_install();
	asm volatile("sti");
	init_timer(50);
	init_keyboard();

	kprint("\n> ");

end_kernel_label:
	goto end_kernel_label;
}

void user_input(char* input)
{
	if (strcmp(input, "END") == 0)
	{
		kprint("Stopping the CPU.\nBye!");
		asm volatile("hlt");
	}
	kprint("You said: ");
	kprint(input);
	kprint("\n> ");
}

