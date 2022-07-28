#include "kernel/kernel.hpp"

#include "cpu/page.hpp"
#include "cpu/acpi.hpp"
#include "cpu/isr.hpp"

#include "klibc/function.hpp"
#include "klibc/memory.hpp"
#include "klibc/kprint.hpp"

#include "klibc/init.hpp"

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
	kprint("Testing interrupts\n");
	__asm__ volatile ("int $3");
	__asm__ volatile ("int $15");

	initKlibc();

	/*
	try {
		throw "C++ Exception Test\n";
	} catch (const char* em) {
		kprint(em);
	}
	*/

	kprint("Starting event loop\n");
kentry_hlt:
	__asm__ ("hlt");
	goto kentry_hlt;
}

extern "C" void exit()
{
	acpiShutdown();
}

void user_input(char input[])
{
	UNUSED(input);
}

