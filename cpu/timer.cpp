#include "cpu/timer.hpp"
#include "cpu/isr.hpp"
#include "cpu/io.hpp"

#include "klibc/function.hpp"

unsigned tick = 0;


#define APIC_LVT_VECTOR(x) (x & 0xFF)

enum ApicTimerDivisors
{
	APIC_TIMER_DIVISOR_1 = 0xB,
	APIC_TIMER_DIVISOR_2 = 0x0,
	APIC_TIMER_DIVISOR_4 = 0x1,
	APIC_TIMER_DIVISOR_8 = 0x2,
	APIC_TIMER_DIVISOR_16 = 0x3,
	APIC_TIMER_DIVISOR_32 = 0x8,
	APIC_TIMER_DIVISOR_64 = 0x9,
	APIC_TIMER_DIVISOR_128 = 0xA
};

void initTimerApic(uint32_t);
void initTimerPic(uint32_t freq);

static void timerCallback(registers_t *regs)
{
	tick++;
	UNUSED(regs);
}

void initTimer(uint32_t freq)
{
	registerInterruptHandler(IRQ0, timerCallback);
	
	if (checkApic()) initTimerApic(freq);
	else initTimerPic(freq);
}

void initTimerApic(uint32_t)
{
	writeApicRegister(APIC_REGISTER_DIVIDE_CONFIGURATION, APIC_TIMER_DIVISOR_16);
	// pitPrepareSleep(10000);
	
	writeApicRegister(APIC_REGISTER_INITAL_COUNT, UINT32_MAX);

	// pitPerformSleep();
	
	writeApicRegister(APIC_REGISTER_LVT_TIMER, APIC_LVT_MASK);
	uint32_t ticksIn10ms = UINT32_MAX - readApicRegister(APIC_REGISTER_CURRENT_COUNT);
	UNUSED(ticksIn10ms);
	writeApicRegister(APIC_REGISTER_LVT_TIMER, APIC_LVT_VECTOR(IRQ0) | APIC_LVT_TIMER_MODE_PERIODIC);
	writeApicRegister(APIC_REGISTER_DIVIDE_CONFIGURATION, APIC_TIMER_DIVISOR_16);
	writeApicRegister(APIC_REGISTER_INITAL_COUNT, UINT32_MAX);
}

void initTimerPic(uint32_t freq)
{
	uint32_t divisor = 1193180 / freq;
	uint8_t low  = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

	portByteOut(0x43, 0x36);
	portByteOut(0x40, low);
	portByteOut(0x40, high);
}

