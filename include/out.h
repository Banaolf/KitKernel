#pragma once
#include "io.h"
#include <stdint.h>

#define ROW_MAX 24
#define COL_MAX 79

inline uint16_t* VGA = (uint16_t*)0xB8000;
inline uint16_t* vga = (uint16_t*)0x3D4;
inline uint16_t* vga_cursor = (uint16_t*)0x3D5;

//Everything that has to do with writing to terminal/console (host and user)
void serial_print(const char* s);

void kprint_char(const char c, uint8_t co);
void kprint(const char* s);
void kprint(const char* s, uint8_t c);
void kprint_times(int times, const char* s);
void kprint_times(int times, const char* s, uint8_t c);
void kbackspace();
void update_cursor();
void disable_cursor();
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);