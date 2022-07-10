#include "kernel/kernel.hpp"
#include "kernel/kprint.hpp"

#include "cpu/acpi.hpp"
#include "cpu/isr.hpp"

#include "libc/function.hpp"
#include "libc/memory.hpp"

#include "libc/mmap.hpp"

extern "C" void kentry()
{
	kprint("Entered kentry()\n");

	setupPaging();

	initAcpi();

	isrInstall();
	irqInstall();

	kprint("Testing interrupts\n");
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
	sortMemoryMap(&map);

	kprint("Starting event loop\n");
kentry_hlt:
	__asm__ ("hlt");
	goto kentry_hlt;
}

void user_input(char input[])
{
	UNUSED(input);
}

