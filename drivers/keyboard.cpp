#include "drivers/keyboard.hpp"

#include "cpu/io.hpp"
#include "cpu/isr.hpp"

#include "kernel/kernel.hpp"

#include "libc/ctype.hpp"
#include "libc/function.hpp"
#include "libc/memory.hpp"
#include "libc/string.hpp"

// TODO: Remove screen.hpp dependancy. We only want to get the
// input and let the kernel decide whether to print it or not.
#include "drivers/screen.hpp"

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT 0x2A

static char key_buffer[256];

static char *key_buffer_overflow = NULL;

#define SC_MAX 57
const char *sc_name[] = { "ERROR", "Esc", "1", "2", "3", "4", "5", "6",
    "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
        "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
        "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
        "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
        "/", "RShift", "Keypad *", "LAlt", "Spacebar"};
const char sc_ascii_lower[] = { '?', '?', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
        'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
        'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};
const char sc_ascii_upper[] = { '?', '?', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
        'U', 'I', 'O', 'P', '{', '}', '?', '?', 'A', 'S', 'D', 'F', 'G',
        'H', 'J', 'K', 'L', ':', '"', '~', '?', '|', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', '<', '>', '?', '?', '?', '?', ' '};

bool async_key_states[0x7F];
bool capslock = false;

static void keyboardCallback(registers_t *regs)
{
	unsigned char scancode = portByteIn(0x60);
	
	bool release = scancode > 0x7F;
	if (release) async_key_states[scancode-0x80] = false;
	else         async_key_states[scancode]      = true;

	if (scancode == 0x3A) capslock ^= 1;

	if (scancode > SC_MAX) return;
	if (scancode == BACKSPACE)
	{
		if (key_buffer[0] == '\0') return;
		backspace(key_buffer);
		kprint_backspace();
	}
	else if (scancode == ENTER)
	{
		kprint("\n");
		user_input(key_buffer);
		key_buffer[0] = '\0';
	}
	else if (scancode == LSHIFT);
	else
	{
		if (sc_ascii_lower[(int)scancode] == '?' || sc_ascii_lower[(int)scancode] == '\0') return;
		//if (key_buffer[256] != '\0') { key_buffer_overflow = (char *)kmalloc(2048-256); }
		UNUSED(key_buffer_overflow);
		if ((async_key_states[LSHIFT] && !capslock) || (capslock && !async_key_states[LSHIFT]))
		{
			char letter = sc_ascii_upper[(int)scancode];
			if (capslock && isalpha(letter)) letter = sc_ascii_lower[(int)scancode] + 26;
			else letter = sc_ascii_lower[(int)scancode];
			char str[2] = { letter, '\0' };
			append(key_buffer, letter);
			kprint(str);
		}
		else
		{
			char letter = sc_ascii_lower[(int)scancode];
			char str[2] = { letter, '\0' };
			append(key_buffer, letter);
			kprint(str);
		}
	}
	UNUSED(regs);
}

bool get_async_key_state(uint32_t k)
{
	if (!(k < 1 || k > 58)) return -1;
	else                    return async_key_states[k];
}

int initKeyboard()
{
	registerInterruptHandler(IRQ1, keyboardCallback);
	for (uint64_t i = 0; i > sizeof(async_key_states)/sizeof(bool); i++)
	{
		async_key_states[i] = false;
	}
	
	return 0;
}

