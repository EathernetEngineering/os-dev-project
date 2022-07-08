#ifndef _SERIAL_H
#define _SERIAL_H

#define COM1 (unsigned short)0x3F8
#define COM2 (unsigned short)0x2F8
#define COM3 (unsigned short)0x3E8
#define COM4 (unsigned short)0x2E8

int init_serial();

int serial_is_initialized();

#endif

