#ifndef __DRIVERS__SCREEN_H__
#define __DRIVERS__SCREEN_H__

#include <stddef.h>

#define VIDEO_ADDRESS		(unsigned char*)0xB8000
#define MAX_ROWS			25
#define MAX_COLS			80
#define GREY_ON_BLACK		0x07
#define WHITE_ON_BLACK		0x0F
#define RED_ON_BLACK		0x04
#define WHITE_ON_RED		0xF4
#define RED_ON_WHITE		0x4F
#define YELLOW_ON_BLACK		0x0E

/* Screen i/o ports */
#define REG_SCREEN_CTRL  0x3d4
#define REG_SCREEN_DATA  0x3d5

/* Public kernel API */
void clear_screen();
void kprint_at(const char *message, int col, int row);
void kerr_at(const char *message, int col, int row);
void kcrit_at(const char *message, int col, int row);
void kprint(const char *message);
void kwarn(const char *message);
void kerr(const char *message);
void kcrit(const char *message);

void kprint_backspace();

#endif

