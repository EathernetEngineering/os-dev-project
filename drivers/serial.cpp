#include "drivers/serial.hpp"
#include "drivers/screen.hpp"

#include "cpu/io.hpp"
#include "cpu/isr.hpp"

#include "libc/function.hpp"

int serial_initialized = 0;

static void serialInCallback(registers_t *regs)
{
	char serial_in = portByteIn(COM1);

	kprint("char recived from serial in: ");
	char byte_str[] = { serial_in, '\0' };
	kprint(byte_str);
	kprint("\n");

	UNUSED(regs);
}

int initSerial()
{
	if (serial_initialized > 0) return 2;

	int fail = 1;

	portByteOut(COM1 + 1, (unsigned char)0x00);
	portByteOut(COM1 + 3, (unsigned char)0x80);
	portByteOut(COM1 + 0, (unsigned char)0x03);
	portByteOut(COM1 + 1, (unsigned char)0x00);
	portByteOut(COM1 + 3, (unsigned char)0x03);
	portByteOut(COM1 + 2, (unsigned char)0xC7);
	portByteOut(COM1 + 4, (unsigned char)0x0B);
	portByteOut(COM1 + 4, (unsigned char)0x1E);
	portByteOut(COM1 + 0, (unsigned char)'S');

	unsigned char loop_byte = portByteIn(COM1);
	if (loop_byte != (unsigned char)'S')
	{
		return fail;
	}
	else
	{
		fail = 0;
		serial_initialized = 1;
	}

	registerInterruptHandler(IRQ4, serialInCallback);
	portByteOut(COM1 + 4, 0x0F);

	return fail;
}

int serial_is_initialized()
{
	return serial_initialized;
}

