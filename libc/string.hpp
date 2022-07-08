#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t length);
void *memset(void *dest,char val, size_t length);

void reverse_str(char s[]);
int strlen(char s[]);
void backspace(char s[]);
void append(char s[], char n);
int memcmp(const void *ptr1, const void *ptr2, size_t n);
int strcmp(char s1[], char s2[]);
void int_to_ascii(int n, char str[]);
void hex_to_ascii(int n, char str[]);

#define ZeroMemory(ptr, length) memset(ptr, 0, length)

#endif

