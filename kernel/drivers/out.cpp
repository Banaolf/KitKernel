#include "../../include/out.h"
#include <stdint.h>

static int row = 0;
static int col = 0;
static uint16_t pos = row * 80 + col;

static inline void update_pos() {pos = row * 80 + col;}

/*out.h: Update the VGA cursor*/
void update_cursor(int x, int y) {
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

static void scroll() {
    for (int y = 1; y < ROW_MAX; y++) {
        for (int x = 0; x < 80; x++) {
            VGA[(y - 1) * 80 + x] = VGA[y * 80 + x];
        }
    }
    for (int x = 0; x < 80; x++) {
        VGA[(ROW_MAX - 1) * 80 + x] = (uint16_t)' ' | ((uint16_t)0x0F << 8);
    }
    row = ROW_MAX - 1;
    col = 0;
}

static void kprint_char(const char c, uint8_t co) {
    if (c == '\n') {
        row++;
        col = 0;
    }
    VGA[pos] = (uint16_t)c | ((uint16_t) co << 8);
    col++;
    if (col > COL_MAX) {row++; col = 0;}
    if (row > ROW_MAX) {scroll();}
    update_pos();
}

/*out.h: Print a string*/
void kprint(const char* s) {
    for (int idx = 0; s[idx] != '\0'; idx++) {
        kprint_char(s[idx], 0x70);
    }
}

/*out.h: Print a string with color!*/
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

void kbackspace() {
    if (col == 0) {
        if (row == 0) return;
        row--;
        col = COL_MAX;
    } else col--;
    VGA[pos] = (uint16_t)' ' | ((uint16_t)0x0F << 8);
    update_pos();
}