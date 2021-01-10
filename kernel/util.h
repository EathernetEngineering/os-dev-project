#ifndef __KERNEL__UTIL_H__
#define __KERNEL__UTIL_H__

#define LOWORD(address) (unsigned short)((address) & 0xFFFF)
#define HIWORD(address) (unsigned short)((address >> 16) & 0xFFFF)

#endif

