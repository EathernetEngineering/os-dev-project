#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"
#include "../cpu/timer.h"
#include "../libc/string.h"
#include "../libc/stddef.h"

bool should_quit = false;

void _start()
{
	kprint_at("Loaded Kernel!", 0, 1);
	isr_install();
	asm volatile("sti");
	init_timer(50);
	init_keyboard();

	kprint("\n> ");

	while (!should_quit);
}

void user_input(char* input)
{
	if (strcmp(input, "END") == 0)
	{
		kprint("Stopping the CPU.\nBye!");
		should_quit = true;
	}
	kprint("You said: ");
	kprint(input);
	kprint("\n> ");
}

