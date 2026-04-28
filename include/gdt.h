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

struct TSS {
    uint32_t reserved0;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved1;
    uint64_t ist[7];
    uint64_t reserved2;
    uint16_t reserved3;
    uint16_t iopb_offset;
} __attribute__((packed));

struct TSSDescriptor {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t flags1;
    uint8_t flags2;
    uint8_t base_high;
    uint32_t base_upper;
    uint32_t reserved;
} __attribute__((packed));
inline TSS kernel_tss;

#define IST_CURRENT_ALLOCATED 3

void install_tss(uint64_t tss_addr);
void tss_init();
void init_gdt();