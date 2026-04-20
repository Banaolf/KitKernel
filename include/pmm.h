#pragma once
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

struct page_table {
    uint64_t entries[512];
} __attribute__((aligned(4096)));

enum PAGE_FLAGS {
    PRESENT   = 1ULL << 0,
    WRITABLE  = 1ULL << 1,
    USER      = 1ULL << 2,
    WRITE_THROUGH = 1ULL << 3,
    NO_CACHE  = 1ULL << 4,
    HUGE_PAGE = 1ULL << 7,
};

struct multiboot_tag_framebuffer {
    uint32_t type;
    uint32_t size;
    uint64_t addr;
    uint32_t pitch;
    uint32_t width;
    uint32_t height;
    uint8_t bpp;
    uint8_t framebuffer_type;
    uint8_t reserved;
};

struct mmap_entry {
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t zero;
};

void pmm_init(uint64_t* multiboot_addr);
void* kmalloc_page(size_t size);
void kfree_page(void* addr, size_t size);

uint64_t get_memupper();

void* memset(void* dest, int value, size_t count);
void zero_page(void* page_phys_addr);
void map_page(uint64_t pml4_phys, uint64_t virt, uint64_t phys, uint64_t flags);
void map_framebuffer(uint64_t pml4_phys, multiboot_tag_framebuffer* fb);