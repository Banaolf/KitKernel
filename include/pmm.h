#pragma once
#include <cstdint>
#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE 4096

struct bitmap {
    uint64_t* bitmap;
    uint64_t total_pages;
    uint64_t start;
    uint64_t end;
    uint64_t size;
};

struct multiboot_tag {
    uint32_t type;
    uint32_t size;
};

struct mmap_entry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t zero;
};

void pmm_init(uint64_t mem_size, uint64_t* multiboot_address);
void* kmalloc_page(size_t size);
void kfree_page(void* obj);