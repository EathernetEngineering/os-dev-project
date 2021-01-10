#include "timer.h"
#include "../drivers/screen.h"
#include "ports.h"
#include "../kernel/util.h"
#include "isr.h"
#include "../libc/function.h"

unsigned tick =0;

static void timer_callback(registers_t regs)
{
	tick++;
	UNUSED(regs);
}

void init_timer(unsigned freq)
{
	register_interrupt_handler(IRQ0, timer_callback);

	unsigned divisor = 1193180 / freq;
	unsigned char low  = (unsigned char)(divisor & 0xFF);
	unsigned char high = (unsigned char)((divisor >> 8) & 0xFF);

	port_byte_out(0x43, 0x36);
	port_byte_out(0x40, low);
	port_byte_out(0x40, high);
}

