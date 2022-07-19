#include "klibc/string.hpp"

void bcopy(const void *src, void *dest, size_t len)
{
	if (dest < src)
	{
		const char *firsts = (const char*)src;
		char *firstd = (char*)dest;
		while (len--)
			*firstd++ = *firsts++;
	}
	else
	{
		const char *lasts = (const char*)src + (len-1);
		char *lastd = (char*)dest + (len-1);
		while (len--)
			*lastd-- = *lasts--;
	}
}

void *memcpy(void *out, const void *in, size_t length)
{
	bcopy(in, out, length);
	return out;
}

void *memset(void *dest, char val, size_t length)
{
	unsigned char *ptr = (unsigned char*)dest;
	while (length-- > 0)
		*ptr++ = val;
	return dest;
}

void reverse_str(char s[])
{
	int c, i, j;
	for (i = 0, j = strlen(s)-1; i < j; i++, j--)
	{
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}

int strlen(char s[])
{
	int i = 0;
	while (s[i] != '\0') ++i;
	return i;
}

void append(char s[], char n)
{
	int len = strlen(s);
	s[len] = n;
	s[len+1] = '\0';
}

void backspace(char s[])
{
	int len = strlen(s);
	s[len-1] = '\0';
}

int memcmp(const void *ptr1, const void *ptr2, size_t n)
{
	if (ptr1 == ptr2) return 0;
	while (n--)
		if (*reinterpret_cast<const char *>(ptr1) != *reinterpret_cast<const char *>(ptr2))
			return *reinterpret_cast<const char *>(ptr1) - *reinterpret_cast<const char *>(ptr2);
		else reinterpret_cast<const char *&>(ptr1)++, reinterpret_cast<const char *&>(ptr2)++;
	return 0;
}

int strcmp(char s1[], char s2[])
{
	if (s1 == s2) return 0;
	int i;
	for (i = 0; s1[i] == s2[i]; i++)
	{
		if (s1[i] == '\0') return 0;
	}
	return s1[i] - s2[i];
}

void int_to_ascii(int n, char str[])
{
	int i, sign;
	if ((sign = n) < 0) n = -n;
	i = 0;
	do {
		str[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	if (sign < 0) str[i++] = '-';
	str[i] = 0;

	reverse_str(str);
}

void hex_to_ascii(int n, char str[])
{
	append(str, '0');
	append(str, 'x');
	char zeros = 0;

	int tmp;
	int i;
	for (i = 28; i > 0; i -= 4)
	{
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && zeros == 0) continue;
		zeros = 1;
		if (tmp > 0xA) append(str, tmp - 0xA + 'A');
		else append(str, tmp + '0');
	}
	tmp = n & 0xF;
	if (tmp >= 0xA) append(str, tmp - 0xA + 'A');
	else append(str, tmp + '0');
}

