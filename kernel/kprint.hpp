#ifndef _KERNEL_KPRINT_H
#define _KERNEL_KPRINT_H

#include <stddef.h>

//static constexpr uint8_t *s_VgaBaseAddr    = (uint8_t*)(void*)0xB8000;
#define s_VgaBaseAddr reinterpret_cast<uint8_t *>(0xB8000)
static constexpr uint32_t s_VgaMaxRows     = 25;
static constexpr uint32_t s_VgaMaxCols     = 80;

typedef enum VgaColors : uint8_t
{
	VGA_COLOR_BLACK            = 0x00,
	VGA_COLOR_BLUE             = 0x01,
	VGA_COLOR_GREEN            = 0x02,
	VGA_COLOR_CYAN             = 0x03,
	VGA_COLOR_RED              = 0x04,
	VGA_COLOR_MAGENTA          = 0x05,
	VGA_COLOR_BROWN            = 0x06,
	VGA_COLOR_LIGHT_GRAY       = 0x07,
	VGA_COLOR_DARK_GRAY        = 0x08,
	VGA_COLOR_LIGHT_BLUE       = 0x09,
	VGA_COLOR_LIGHT_GREEN      = 0x0A,
	VGA_COLOR_LIGHT_CYAN       = 0x0B,
	VGA_COLOR_LIGHT_RED        = 0x0C,
	VGA_COLOR_LIGHT_MAGENTA    = 0x0D,
	VGA_COLOR_YELLOW           = 0x0E,
	VGA_COLOR_WHITE            = 0x0F
} vga_color_t;

constexpr vga_color_t vgaColorBackground(vga_color_t color)
{
	return static_cast<vga_color_t>(color << 4);
}

constexpr vga_color_t vgaColorForeground(vga_color_t color)
{
	return static_cast<vga_color_t>(color);
}

constexpr vga_color_t vgaColor(vga_color_t forground, vga_color_t background)
{
	return static_cast<vga_color_t>(forground | (background << 4));
}

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
