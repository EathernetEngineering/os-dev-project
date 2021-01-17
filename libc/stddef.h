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

#if __WORDSIZE == 64
#ifndef __intptr_t_defined
typedef long int				intptr_t;
#define __intptr_t_defined
#endif
typedef unsigned long int		uintptr_t;
#else
#ifndef __intptr_t_defined
typedef int						intptr_t;
#define __intptr_t_defined
#endif
typedef unsigned int			uintptr_t;
#endif

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

#define NULL 0L

#define __bool_true_false_are_defined 1

#define __DEV_T_TYPE			__UQUAD_TYPE
#define __UID_T_TYPE			__U32_TYPE
#define __GID_T_TYPE			__U32_TYPE
#define __INO_T_TYPE			__ULONGWORD_TYPE
#define __INO64_T_TYPE			__UQUAD_TYPE
#define __MODE_T_TYPE			__U32_TYPE
#define __NLINK_T_TYPE			__UWORD_TYPE
#define __OFF_T_TYPE			__SLONGWORD_TYPE
#define __OFF64_T_TYPE			__SQUAD_TYPE
#define __PID_T_TYPE			__S32_TYPE
#define __RLIM_T_TYPE			__ULONGWORD_TYPE
#define __RLIM64_T_TYPE			__UQUAD_TYPE
#define	__BLKCNT_T_TYPE			__SLONGWORD_TYPE
#define	__BLKCNT64_T_TYPE		__SQUAD_TYPE
#define	__FSBLKCNT_T_TYPE		__ULONGWORD_TYPE
#define	__FSBLKCNT64_T_TYPE		__UQUAD_TYPE
#define	__FSFILCNT_T_TYPE		__ULONGWORD_TYPE
#define	__FSFILCNT64_T_TYPE		__UQUAD_TYPE
#define	__FSWORD_T_TYPE			__SWORD_TYPE
#define	__ID_T_TYPE				__U32_TYPE
#define __CLOCK_T_TYPE			__SLONGWORD_TYPE
#define __TIME_T_TYPE			__SLONGWORD_TYPE
#define __USECONDS_T_TYPE		__U32_TYPE
#define __SUSECONDS_T_TYPE		__SLONGWORD_TYPE
#define __DADDR_T_TYPE			__S32_TYPE
#define __KEY_T_TYPE			__S32_TYPE
#define __CLOCKID_T_TYPE		__S32_TYPE
#define __TIMER_T_TYPE			void *
#define __BLKSIZE_T_TYPE		__SLONGWORD_TYPE
#define __FSID_T_TYPE			struct { int __val[2]; }
#define __SSIZE_T_TYPE			__SWORD_TYPE
#define __SYSCALL_SLONG_TYPE	__SLONGWORD_TYPE
#define __SYSCALL_ULONG_TYPE	__ULONGWORD_TYPE
#define __CPU_MASK_TYPE			__ULONGWORD_TYPE

#define __FD_SETSIZE			1024

#define __SIZE_TYPE__			long unsigned int

typedef __SIZE_TYPE__			size_t;

#define offsetof(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)

#define container_of(ptr, type, member) ({ \
                const typeof( ((type *)0)->member ) *__mptr = (ptr); \
                (type *)( (char *)__mptr - offsetof(type,member) );})

#endif

