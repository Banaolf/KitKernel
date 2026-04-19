#pragma once
#include "io.h"
#include <stdint.h>

#define ROW_MAX 24
#define COL_MAX 79

inline uint16_t* VGA = (uint16_t*)0xB8000;
inline uint16_t* vga = (uint16_t*)0x3D4;
inline uint16_t* vga_cursor = (uint16_t*)0x3D5;

//Everything that has to do with writing to terminal/console (host and user)
void inline serial_print(const char* s) {
    for (int i = 0; s[i] != '\0'; i++) {
        // Wait for the transmit buffer to be empty before sending
        while ((inb(0x3F8 + 5) & 0x20) == 0); 
        outb(0x3F8, s[i]);
    }
}

void kprint(const char* s);
void kprint(const char* s, uint8_t c);
void kprint_times(int times, const char* s);
void kprint_times(int times, const char* s, uint8_t c);
void kbackspace();