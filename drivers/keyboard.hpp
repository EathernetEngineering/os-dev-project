#ifndef __DRIVERS__KEYBOARD_H__
#define __DRIVERS__KEYBOARD_H__

enum keycode
{
	KEY_ESC = 1,
	KEY_1 = 2,
	KEY_2 = 3,
	KEY_3 = 4,
	KEY_4 = 5,
	KEY_5 = 6,
	KEY_6 = 7,
	KEY_7 = 8,
	KEY_8 = 9,
	KEY_9 = 10,
	KEY_0 = 11,
	KEY_MINUS = 12,
	KEY_EQUALS = 13,
	KEY_BACKSPACE = 14,
	KEY_TAB = 15,
	KEY_Q = 16,
	KEY_W = 17,
	KEY_E = 18,
	KEY_R = 19,
	KEY_T = 20,
	KEY_Y = 21,
	KEY_U = 22,
	KEY_I = 23,
	KEY_O = 24,
	KEY_P = 25,
	KEY_LBRACKET = 26,
	KEY_RBRACKET = 27,
	KEY_ENTER = 28,
	KEY_LCTRL = 29,
	KEY_A = 30,
	KEY_S = 31,
	KEY_D = 32,
	KEY_F = 33,
	KEY_G = 34,
	KEY_H = 35,
	KEY_J = 36,
	KEY_K = 37,
	KEY_L = 38,
	KEY_SEMICOLON = 39,
	KEY_QUOTE = 40,
	KEY_GRAVE = 41,
	KEY_LSHIFT = 42,
	KEY_BACKSLASH = 43,
	KEY_Z = 44,
	KEY_X = 45,
	KEY_C = 46,
	KEY_V = 47,
	KEY_B = 48,
	KEY_N = 49,
	KEY_M = 50,
	KEY_COMMA = 51,
	KEY_PERIOD = 52,
	KEY_FORWARDSLASH = 53,
	KEY_RSHIFT = 54,
	KEY_KEYPADASTRICKS = 55,
	KEY_LALT = 56,
	KEY_SPACEBAR = 57,
	KEY_CAPSLOCK = 58
};

int init_keyboard();

bool get_async_key_state(uint32_t k);

#endif

