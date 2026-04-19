#pragma once
#include <stdint.h>

struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  attributes;
    uint8_t  base_high;
} __attribute__((packed));

struct GDTDescriptor {
    uint16_t size;
    uint64_t offset;
} __attribute__((packed));

void init_gdt();