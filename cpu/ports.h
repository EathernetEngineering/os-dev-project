#ifndef __DRIVERS__PORTS__H__
#define __DRIVERS__PORTS_H__

unsigned char port_byte_in(unsigned short port);
void port_byte_out(unsigned short port, unsigned char data);
unsigned short port_word_in(unsigned short port);
void port_word_out(unsigned short port, unsigned short data);

#endif

