#include "../../include/pmm.h"
#include "../../include/out.h"
#include "../../include/gdt.h"
#include <stdint.h>

static struct bitmap Bitmap;
uint64_t mem_upper = 0;
multiboot_tag_framebuffer* global_fb = nullptr;

void* memset(void* dest, int value, size_t count) {
    uint8_t* ptr = (uint8_t*)dest;
    while (count--) {
        *ptr++ = (uint8_t)value;
    }
    return dest;
}

void zero_page(void* page_phys_addr) {
    uint64_t* ptr = (uint64_t*)page_phys_addr;
    for (int i = 0; i < 512; i++) {
        ptr[i] = 0;
    }
}

static void pmm_free_page(uint64_t address) {
    uint64_t page_index = address / 4096;
    uint64_t bitmap_index = page_index / 64;
    uint64_t bit_offset = page_index % 64;
    Bitmap.bitmap[bitmap_index] &= ~(1ULL << bit_offset);
}

static void pmm_lock_page(uint64_t address) {
    uint64_t page_index = address / 4096;
    uint64_t bitmap_index = page_index / 64;
    uint64_t bit_offset = page_index % 64;
    Bitmap.bitmap[bitmap_index] |= (1ULL << bit_offset);
}

//Init the Physical Memory Manager!
void pmm_init(uint64_t* multiboot_addr) {
    serial_print("Assigning first tag\n");
    multiboot_tag* tag = (multiboot_tag*)((uint8_t*)multiboot_addr + 8);
    
    while (tag->type != 0) {
        if (tag->type == 6) {
            serial_print("Found entry type 6!\n");
            uint32_t entry_size = *(uint32_t*)((uint8_t*)tag + 8);
            uint32_t num_entries = (tag->size - 16) / entry_size;
            mmap_entry* ent = (mmap_entry*)((uint8_t*)tag + 16);
            for (uint32_t i = 0; i < num_entries; i++) {
                if ((ent->addr + ent->len) > mem_upper && ent->type == 1) {
                    mem_upper = ent->addr + ent->len;
                    serial_print("Reassigned to mem high\n");
                }
                serial_print("Reassigning entry...\n");
                ent = (mmap_entry*)((uint8_t*)ent + entry_size);
            }
        }
        if (tag->type == 8) {
            global_fb = (multiboot_tag_framebuffer*)tag;
        }
        tag = (multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    serial_print("Got external end, doing bitmap initialising...\n");
    extern char _kernel_end[];
    Bitmap.bitmap = (uint64_t*)((((uint64_t)_kernel_end) + 4095) & ~4095);
    Bitmap.total_pages = mem_upper / PAGE_SIZE;
    Bitmap.size = (Bitmap.total_pages + 63) / 64;
    Bitmap.start = (uint64_t)Bitmap.bitmap;
    Bitmap.end = Bitmap.start + (Bitmap.size * 8);

    serial_print("Marking everything as occupied\n");
    for(uint64_t i = 0; i < Bitmap.size; i++) Bitmap.bitmap[i] = 0xFFFFFFFFFFFFFFFF;

    serial_print("Reassigning tag (again)\n");
    tag = (multiboot_tag*)((uint8_t*)multiboot_addr + 8);
    while (tag->type != 0) {
        if (tag->type == 6) {
            uint32_t entry_size = *(uint32_t*)((uint8_t*)tag + 8);
            uint32_t num_entries = (tag->size - 16) / entry_size;
            mmap_entry* ent = (mmap_entry*)((uint8_t*)tag + 16);
            for (uint32_t i = 0; i < num_entries; i++) {
                if (ent->type == 1) {
                    serial_print("Found free page! Switching bit...\n");
                    for (uint64_t idx = 0; idx < ent->len; idx += PAGE_SIZE) {
                        pmm_free_page(ent->addr + idx);
                    }
                }
                ent = (mmap_entry*)((uint8_t*)ent + entry_size);
            }
        }
        tag = (multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }

    serial_print("Locking vital regions...\n");
    // Lock vital regions
    pmm_lock_page(0);
    extern char _kernel_start[];
    for (uint64_t addr = (uint64_t)_kernel_start; addr < (uint64_t)_kernel_end; addr += PAGE_SIZE) pmm_lock_page(addr);
    for (uint64_t addr = Bitmap.start; addr < Bitmap.end; addr += PAGE_SIZE) pmm_lock_page(addr);
    serial_print("Locking multiboot address\n");
    uint32_t mb_size = *(uint32_t*)multiboot_addr;
    for (uint64_t addr = (uint64_t)multiboot_addr; addr < (uint64_t)multiboot_addr + mb_size; addr += PAGE_SIZE) pmm_lock_page(addr);
}

static bool is_bit_free(uint64_t page_index) {
    return !(Bitmap.bitmap[page_index / 64] & (1ULL << (page_index % 64)));
}
static uint64_t last_found = 0;
void* kmalloc_page(size_t size) {
    serial_print("Calculating...\n");
    uint64_t pages_needed = (size + 4095) / 4096;
    uint64_t consecutive_found = 0;

    for (uint64_t i = last_found; i < Bitmap.total_pages; i++) {
        if (is_bit_free(i)) consecutive_found++;
        else consecutive_found = 0;

        if (consecutive_found == pages_needed) {
            serial_print("Found match!\n");
            uint64_t start_page = i - pages_needed + 1;
            for (uint64_t j = start_page; j <= i; j++) pmm_lock_page(j * PAGE_SIZE);
            last_found = i+1;
            return (void*)(start_page * 4096);
        }
    }
    serial_print("Didn't find match.\n");
    if (last_found != 0) last_found = 0;
    return nullptr;
}

void kfree_page(void* addr, size_t size) {
    serial_print("Freeing page...\n");
    uint64_t phys_addr = (uint64_t)addr;
    uint64_t pages_to_free = (size + 4095) / 4096;
    for (uint64_t i = 0; i < pages_to_free; i++) pmm_free_page(phys_addr + (i * 4096));
}

uint64_t get_memupper() { return mem_upper; }

void tss_init() {
    memset(&kernel_tss, 0, sizeof(TSS));

    for (int i = 0; i < IST_CURRENT_ALLOCATED; i++) {
        void* stack = kmalloc_page(PAGE_SIZE);
        kernel_tss.ist[i] = (uint64_t)stack + PAGE_SIZE;
    }

    install_tss((uint64_t)&kernel_tss);
}