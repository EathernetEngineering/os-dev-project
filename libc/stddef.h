#ifndef __LIBC__STDDEF_H__
#define __LIBC__STDDEF_H__

typedef unsigned char __uint8_t;
typedef signed char __int8_t;
typedef unsigned short int __uint16_t;
typedef signed short int __int16_t;
typedef unsigned int __uint32_t;
typedef signed int __int32_t;
#if __WORDSIZE == 64
typedef unsigned long int __uint64_t;
typedef signed long int __int64_t;
#else
typedef unsigned long long int __uint64_t;
typedef signed long long int __int64_t;
#endif

typedef __uint8_t  uint8_t;
typedef __int8_t   int8_t;
typedef __uint16_t uint16_t;
typedef __int16_t  int16_t;
typedef __uint32_t uint32_t;
typedef __int32_t  int32_t;
typedef __uint64_t uint64_t;
typedef __int64_t  int64_t;

#ifndef __cplusplus
#define bool _Bool
#define true 1
#define false 0
#elif defined(__GNUC__) && !defined(__STRICT_ANSI__)
#define Bool bool
#if __cplusplus < 201103L
#define bool bool
#define false false
#define true true
#endif
#endif

#define __bool_true_false_are_defined 1

#endif

