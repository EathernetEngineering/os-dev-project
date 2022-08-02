#include "terminal/screen.hpp"

uint8_t portByteIn(uint16_t port);
void portByteOut(uint16_t port, uint8_t data);

void memcpy(void *dst, void *src, size_t length);

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
		offset = print_char(message[i++], col, row, GREY_ON_BLACK);

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
		offset = print_char(message[i++], col, row, YELLOW_ON_BLACK);

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
		offset = print_char(message[i++], col, row, RED_ON_BLACK);

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
		offset = print_char(message[i++], col, row, WHITE_ON_RED);

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
	print_char(' ', col, row, WHITE_ON_BLACK);
	set_cursor_offset(offset);
}

int print_char(char c, int col, int row, char attr) {
	static unsigned char *vidmem =  VIDEO_ADDRESS;
	if (!attr) attr = GREY_ON_BLACK;

	if (col >= MAX_COLS || row >= MAX_ROWS) {
		vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
		vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = WHITE_ON_RED;
		return get_offset(col, row);
	}

	int offset;
	if (col >= 0 && row >= 0) offset = get_offset(col, row);
	else offset = get_cursor_offset();

	if (c == '\n') {
		row = get_offset_row(offset);
		offset = get_offset(0, row+1);
		
	} else if (c == '\r') {
		row = get_offset_row(offset);
		offset = get_offset(0, row);
	} else if (c == '\t') {
		for (uint32_t i = 0; i < 8; i += 2)
		{
			vidmem[offset] = ' ';
			vidmem[offset + 1] = attr;
			offset += 2;
		}
	} else {
		vidmem[offset] = c;
		vidmem[offset+1] = attr;
		offset += 2;
	}

	if (offset >= MAX_ROWS * MAX_COLS * 2)
	{
		unsigned int i = 0;
		while(i++ < MAX_ROWS)
			memcpy(get_offset(0, i-1) + VIDEO_ADDRESS,
					get_offset(0, i) + VIDEO_ADDRESS,
					MAX_COLS * 2);

		char *last_line = (char *)(get_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS);
		for (i = 0; i < MAX_COLS * 2; i++) last_line[i] = 0;

		offset -= 2 * MAX_COLS;
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
	int screen_size = MAX_COLS * MAX_ROWS;
	int i;
	unsigned char *screen = VIDEO_ADDRESS;

	for (i = 0; i < screen_size; i++) {
		screen[i*2] = ' ';
		screen[i*2+1] = WHITE_ON_BLACK;
	}
	set_cursor_offset(get_offset(0, 0));
}

int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }

