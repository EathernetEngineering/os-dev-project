#include "kernel/kernel.hpp"

#include "cpu/acpi.hpp"
#include "cpu/isr.hpp"

#include "drivers/screen.hpp"

#include "libc/function.hpp"
#include "libc/memory.hpp"

extern "C" void kentry()
{
	kprint("Entered kernel.\n");
	setupPaging();

	initAcpi();

	isrInstall();
	irqInstall();

	kprint("Testing interrupts..\n\n");
	__asm__ volatile ("int $3");
	__asm__ volatile ("int $15");

	/*
	try {
		throw "C++ Exception Test\n";
	} catch (const char* em) {
		kprint(em);
	}
	*/

	MemoryMap map;
	UNUSED(map);

	kprint("\nEntering event loop.\n");
kentry_hlt:
	__asm__ ("hlt");
	goto kentry_hlt;
}

void user_input(char input[])
{
	UNUSED(input);
}

