#include "kernel/kernel.hpp"

#include "cpu/page.hpp"
#include "cpu/acpi.hpp"
#include "cpu/isr.hpp"

#include "klibc/function.hpp"
#include "klibc/memory.hpp"
#include "klibc/init.hpp"
#include "klibc/stdlib.hpp"

#include "drivers/screen.hpp"

extern "C" void kstart()
{
	kprint("Entered kentry()\n");

	isrInstall();
	setupPaging();
	initAcpi();
	irqInstall();
}

extern "C" void kmain()
{
	initKlibc();

	kprint("Starting event loop\n");
	for (;;) {
		__asm__ ("hlt");
	}
}

extern "C" void exit()
{
	acpiShutdown();
}

void user_input(char input[])
{
	UNUSED(input);
}

