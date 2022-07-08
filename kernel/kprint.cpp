#include <kernel/kprint.hpp>

#include <stddef.h>
#include <libc/string.hpp>
#include <libc/memory.hpp>

#include <cpu/io.hpp>

int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int row);
int get_offset_col(int col);

void kprint_at(const char *message, int col, int row)
{
	int offset;
	if (col >=0 && row >= 0)
		offset = get_offset(col, row);
	else
	{
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
	
	int i = 0;
	while (message[i] != 0x00)
	{
		offset = print_char(message[i++], col, row, vgaColor(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));

		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void kwarn_at(const char *message, int col, int row)
{
	int offset;
	if (col >=0 && row >= 0)
		offset = get_offset(col, row);
	else
	{
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
	
	int i = 0;
	while (message[i] != 0x00)
	{
		offset = print_char(message[i++], col, row, vgaColor(VGA_COLOR_YELLOW, VGA_COLOR_BLACK));

		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void kerr_at(const char *message, int col, int row)
{
	int offset;
	if (col >=0 && row >= 0)
		offset = get_offset(col, row);
	else
	{
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
	
	int i = 0;
	while (message[i] != 0x00)
	{
		offset = print_char(message[i++], col, row, vgaColor(VGA_COLOR_RED, VGA_COLOR_BLACK));

		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void kcrit_at(const char *message, int col, int row)
{
	int offset;
	if (col >=0 && row >= 0)
		offset = get_offset(col, row);
	else
	{
		offset = get_cursor_offset();
		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
	
	int i = 0;
	while (message[i] != 0x00)
	{
		offset = print_char(message[i++], col, row, vgaColor(VGA_COLOR_WHITE, VGA_COLOR_RED));

		row = get_offset_row(offset);
		col = get_offset_col(offset);
	}
}

void kprint(const char *message)
{
	kprint_at(message, -1, -1);
}

void kwarn(const char *message)
{
	kwarn_at(message, -1, -1);
}

void kerr(const char *message)
{
	kerr_at(message, -1, -1);
}

void kcrit(const char *message)
{
	kcrit_at(message, -1, -1);
}

void kprint_backspace()
{
	int offset = get_cursor_offset()-2;
	int row = get_offset_row(offset);
	int col = get_offset_col(offset);
	//portByteOut(COM1, '\b');
	print_char(' ', col, row, vgaColor(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK));
	//portByteOut(COM1, '\b');
	set_cursor_offset(offset);
}

int print_char(char c, int col, int row, char attr) {
	static unsigned char *vidmem =  s_VgaBaseAddr;
	if (!attr) attr = vgaColor(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);

	if (col >= static_cast<int>(s_VgaMaxCols) || row >= static_cast<int>(s_VgaMaxRows)) {
		vidmem[2*(s_VgaMaxCols)*(s_VgaMaxRows)-2] = 'E';
		vidmem[2*(s_VgaMaxCols)*(s_VgaMaxCols)-1] = vgaColor(VGA_COLOR_RED, VGA_COLOR_WHITE);
		return get_offset(col, row);
	}

	int offset;
	if (col >= 0 && row >= 0) offset = get_offset(col, row);
	else offset = get_cursor_offset();

	if (c == '\n') {
		row = get_offset_row(offset);
		offset = get_offset(0, row+1);
		
		/*if (serial_is_initialized())
		{
			portByteOut(COM1, '\r');
			portByteOut(COM1, '\n');
		}*/
	} else if (c == '\r') {
		row = get_offset_row(offset);
		offset = get_offset(0, row);
		/*if (serial_is_initialized())
		{
			portByteOut(COM1, '\r');
		}*/
	} else if (c == '\t') {
		for (uint32_t i = 0; i < 8; i += 2)
		{
			vidmem[offset] = ' ';
			vidmem[offset + 1] = attr;
			offset += 2;
		}
		/*if (serial_is_initialized())
		{
			portByteOut(COM1, '\t');
		}*/
	} else {
		vidmem[offset] = c;
		vidmem[offset+1] = attr;
		offset += 2;
		/*if (serial_is_initialized())
		{
			portByteOut(COM1, c);
		}*/
	}

	if (offset >= static_cast<int>(s_VgaMaxRows * s_VgaMaxCols * 2))
	{
		unsigned int i = 0;
		while(i++ < s_VgaMaxRows)
			memcpy(get_offset(0, i-1) + s_VgaBaseAddr,
					get_offset(0, i) + s_VgaBaseAddr,
					s_VgaMaxCols * 2);

		char *last_line = (char *)(get_offset(0, s_VgaMaxRows-1) + s_VgaBaseAddr);
		for (i = 0; i < s_VgaMaxCols * 2; i++) last_line[i] = 0;

		offset -= 2 * s_VgaMaxCols;
	}

	set_cursor_offset(offset);
	return offset;
}

int get_cursor_offset() {
	portByteOut(REG_SCREEN_CTRL, 14);
	int offset = portByteIn(REG_SCREEN_DATA) << 8;
	portByteOut(REG_SCREEN_CTRL, 15);
	offset += portByteIn(REG_SCREEN_DATA);
	return offset * 2;
}

void set_cursor_offset(int offset) {
	offset /= 2;
	portByteOut(REG_SCREEN_CTRL, 14);
	portByteOut(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	portByteOut(REG_SCREEN_CTRL, 15);
	portByteOut(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
	int screen_size = 2 * s_VgaMaxCols * s_VgaMaxRows;
	unsigned char *screen = s_VgaBaseAddr;

	for (int i = 0; i < screen_size; i += 2) {
		screen[i] = ' ';
		screen[i+1] = vgaColor(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK);
	}
	set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row) { return 2 * (row * s_VgaMaxCols + col); }
int get_offset_row(int offset) { return offset / (2 * s_VgaMaxCols); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*s_VgaMaxCols))/2; }

