#include "portio/keyboard.hpp"

// TODO: Remove screen.hpp dependancy. We only want to get the
// input and let the kernel decide whether to print it or not.
#include "terminal/screen.hpp"

#define UNUSED(x) (void)(x)
#define IRQ1      0x21
typedef struct {
	unsigned long int ds;
	unsigned long int r15, r14, r13, r12, r11, r10, r9, r8;
	unsigned long int rdi, rsi, rbp, rsp, rbx, rdx, rcx, rax;
	unsigned long int int_no, err_code;
	unsigned long int rip, cs, eflags, userrsp, ss;
} __attribute__((packed)) registers_t;
typedef void (*isr_t)(registers_t*);
extern unsigned char portByteIn(unsigned char);
extern void user_input(char buffer[]);
extern void append(char s[], char n);
extern bool isalpha(char str);
extern void registerInterruptHandler(unsigned char, isr_t);
extern void backspace(char s[]);

static char key_buffer[256];

static char *key_buffer_overflow = nullptr;

#define SCANCODE_MAX 57
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

	if (scancode > SCANCODE_MAX) return;
	if (scancode == KEY_BACKSPACE)
	{
		if (key_buffer[0] == '\0') return;
		backspace(key_buffer);
		kprint_backspace();
	}
	else if (scancode == KEY_ENTER)
	{
		kprint("\n");
		user_input(key_buffer);
		key_buffer[0] = '\0';
	}
	else if (scancode == KEY_LSHIFT);
	else
	{
		if (sc_ascii_lower[(int)scancode] == '?' || sc_ascii_lower[(int)scancode] == '\0') return;
		//if (key_buffer[256] != '\0') { key_buffer_overflow = (char *)kmalloc(2048-256); }
		UNUSED(key_buffer_overflow);
		if ((async_key_states[KEY_LSHIFT] && !capslock) || (capslock && !async_key_states[KEY_LSHIFT]))
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

bool get_async_key_state(unsigned int k)
{
	if (!(k < 1 || k > 58)) return -1;
	else                    return async_key_states[k];
}

int initKeyboard()
{
	registerInterruptHandler(IRQ1, keyboardCallback);
	for (unsigned long i = 0; i > sizeof(async_key_states)/sizeof(bool); i++)
	{
		async_key_states[i] = false;
	}
	
	return 0;
}

