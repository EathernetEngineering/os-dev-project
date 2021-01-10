#ifndef __LIBC__STDDEF_H__
#define __LIBC__STDDEF_H__

typedef unsigned char __uint8_t;
typedef unsigned short int __uint16_t;
typedef unsigned int __uint32_t;
#if __WORDSIZE == 64
typedef unsigned long int __uint64_t;
#else
typedef unsigned long long int __uint64_t;
#endif

typedef __uint8_t  uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;


#endif

