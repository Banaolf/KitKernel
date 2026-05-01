#include "../../include/out.h"
#include "../../include/kstring.h"
#include <stdint.h>
#include <stdarg.h>

static int row = 0;
static int col = 0;
static uint16_t pos = row * 80 + col;

static inline void update_pos() {pos = row * 80 + col;}

/*out.h: Update the VGA cursor*/
void update_cursor() {
	update_pos();

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));

	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

/*out.h: Disable the VGA cursor*/
void disable_cursor() {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

/*out.h: Enable the VGA cursor and set it's start and end.*/
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void scroll() {
    for (int y = 1; y < 25; y++) {
        for (int x = 0; x < 80; x++) {
            VGA[(y - 1) * 80 + x] = VGA[y * 80 + x];
        }
    }

    uint16_t blank = (uint16_t)' ' | ((uint16_t)0x0F << 8);
    for (int x = 0; x < 80; x++) {
        VGA[24 * 80 + x] = blank;
    }

    row = 24;
    col = 0;
}

#define TAB_SIZE 4

/*out.h: Print a character!*/
void kprint_char(const char c, uint8_t co) {
	if (c == '\n') {
		row++;
		if (row > ROW_MAX) scroll();
		col = 0;
		update_cursor();
		return;
	} else if (c == '\t')  {
		if (col + TAB_SIZE > COL_MAX) {
			row++;
			if (row > ROW_MAX) scroll();
			int tosum = (col + TAB_SIZE) - COL_MAX;
			col = 0;
			col += tosum;
		} else col+=4;
		update_cursor();
		return;
	} else if (c == '\b') {
		if (col == 0) {
			if (row == 0) return;
			row--;
			col = COL_MAX;
		} else col--;
		VGA[pos] = (uint16_t)' ' | ((uint16_t)0x0F << 8);
		update_cursor();
		return;
	} else if (c == '\x9C') {
		row = 0;
		col = 0;
		update_cursor();
		return;
	}
	else col++;
	VGA[pos] = (uint16_t)c | ((uint16_t)co << 8);
	if (col > COL_MAX) {row++; col = 0;}
	if (row > ROW_MAX) {scroll();}
	update_cursor();
}

/*out.h: Print a string*/
void kprint(const char* s, uint8_t c) {
	for (int idx = 0; s[idx] != '\0'; idx++) {
		kprint_char(s[idx], c);
	}
}

/*out.h: Print a string x amount of times!*/
void kprint_times(const int times, const char* s) {
	for (int i = 0; i < times; i++) {
		kprint(s);
	}
}

/*out.h: Print a string x amount of times with color!*/
void kprint_times(int times, const char* s, uint8_t c) {
	for (int i = 0; i < times; i++) {
		kprint(s, c);
	}
}

/*out.h: Deletes the last character*/
void kbackspace() {
	if (col == 0) {
		if (row == 0) return;
		row--;
		col = COL_MAX;
	} else col--;
	VGA[pos] = (uint16_t)' ' | ((uint16_t)0x0F << 8);
	update_pos();
}

/*out.h: Startup the Serial prints!*/
void serial_init() {
	outb(0x3F8 + 1, 0x00);
	outb(0x3F8 + 3, 0x80);
	outb(0x3F8 + 0, 0x03);
	outb(0x3F8 + 1, 0x00);
	outb(0x3F8 + 3, 0x03);
	outb(0x3F8 + 2, 0xC7);
	outb(0x3F8 + 4, 0x0B);
}

/*out.h: Print... serially!*/
void serial_print(const char *s) {
	for (int i = 0; s[i] != '\0'; i++) {
		while ((inb(0x3F8 + 5) & 0x20) == 0); 
		outb(0x3F8, s[i]);
	}
}

/*out.h: Print a formatted string!*/
void kprintf(const char *__fmt, ...) {
	String s;
	va_list list;
	va_start(list, __fmt);

	s = fmtString(__fmt, list);
	kprint(s.cstr());
	
	va_end(list);
}

/*out.h: Get the current line as a string*/
String kgets() {
	String result;
	int staticol = col;
	col = COMMAND_START;
	while (col <= staticol) {
		result.append(VGA[pos]);
		col++;
		update_pos();
	}
	return result;
}