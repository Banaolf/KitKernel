#include "../../include/pmm.h"
#include <stddef.h>
#include <stdint.h>

static struct bitmap Bitmap;

void* memset(void* dest, int value, size_t count) {
    uint8_t* ptr = (uint8_t*)dest;
    while (count--) {
        *ptr++ = (uint8_t)value;
    }
    return dest;
}

void zero_page(void* page_phys_addr) {
    uint64_t* ptr = (uint64_t*)page_phys_addr;
    // 4096 bytes / 8 bytes per uint64_t = 512 entries
    for (int i = 0; i < 512; i++) {
        ptr[i] = 0;
    }
}

void map_framebuffer(uint64_t pml4_phys, multiboot_tag_framebuffer* fb) {
    uint64_t fb_size_bytes = (uint64_t)fb->pitch * fb->height;

    for (uint64_t i = 0; i < fb_size_bytes; i += 4096) {
        uint64_t phys = fb->addr + i;
        uint64_t virt = fb->addr + i;
        
        map_page(pml4_phys, virt, phys, WRITABLE);
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
static uint64_t mem_upper = 0;
void pmm_init(uint64_t* multiboot_addr) {
	multiboot_tag* tag = (multiboot_tag*)(multiboot_addr + 8);
	while (tag->type != 0) {
		if (tag->type == 6) {
			uint32_t entry_size = *(uint32_t*)((uint8_t*)tag + 8);
			uint32_t num_entries = (tag->size - 16) / entry_size;

			struct mmap_entry* ent = (mmap_entry*)((uint8_t*)tag + 16);
			for (uint32_t i = 0; i < num_entries; i++) {
				if ((ent->addr + ent->len) > mem_upper && ent->type == 1) 
					mem_upper = ent->addr + ent->len;
				
				ent = (mmap_entry*)((uint8_t*)ent + entry_size);
			}
		}
		tag = (multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
	}
	extern char _kernel_end[];
	Bitmap.start = ((uint64_t)_kernel_end + 4095) & ~4095;
	Bitmap.total_pages = mem_upper / PAGE_SIZE;
	Bitmap.size = (Bitmap.total_pages + 63) / 64;
	Bitmap.end = Bitmap.start + (Bitmap.size * 8);

	for(uint64_t i = 0; i < Bitmap.size; i++) {
		Bitmap.bitmap[i] = 0xFFFFFFFFFFFFFFFF;
	}

	tag = (multiboot_tag*)(multiboot_addr + 8);
	while (tag->type != 0) {
		if (tag->type == 6) {
			uint32_t entry_size = *(uint32_t*)((uint8_t*)tag + 8);
			uint32_t num_entries = (tag->size - 16) / entry_size;

			struct mmap_entry* ent = (mmap_entry*)((uint8_t*)tag + 16);
			for (uint32_t i = 0; i < num_entries; i++) {
				if (ent->type == 1) {
					for (uint64_t idx = 0; idx < ent->len; idx+=PAGE_SIZE) {
						pmm_free_page(ent->addr + idx);
					}
				}
				
				ent = (mmap_entry*)((uint8_t*)ent + entry_size);
			}
		}
		tag = (multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
	}
	pmm_lock_page(0);

	extern char _kernel_start[];
	for (uint64_t addr = (uint64_t)_kernel_start; addr < (uint64_t)_kernel_end; addr += PAGE_SIZE) {
		pmm_lock_page(addr);
	}

	for (uint64_t addr = Bitmap.start; addr < Bitmap.end; addr += PAGE_SIZE) {
		pmm_lock_page(addr);
	}

	uint32_t mb_size = *(uint32_t*)multiboot_addr;

	for (uint64_t addr = (uint64_t)multiboot_addr; addr < (uint64_t)multiboot_addr + mb_size; addr += PAGE_SIZE) {
		pmm_lock_page(addr);
	}
}

static bool is_bit_free(uint64_t page_index) {
    uint64_t array_idx = page_index / 64;
    uint64_t bit_offset = page_index % 64;

    return !(Bitmap.bitmap[array_idx] & (1ULL << bit_offset));
}

void* kmalloc_page(size_t size = PAGE_SIZE) {
    uint64_t pages_needed = (size + 4095) / 4096;
    uint64_t consecutive_found = 0;

    for (uint64_t i = 0; i < Bitmap.total_pages; i++) {
        
        if (is_bit_free(i)) {
            consecutive_found++;
        } else {
            consecutive_found = 0;
        }

        if (consecutive_found == pages_needed) {
            uint64_t start_page = i - pages_needed + 1;

            for (uint64_t j = start_page; j <= i; j++) {
                pmm_lock_page(j * PAGE_SIZE);
            }

            return (void*)(start_page * 4096);
        }
    }

    return nullptr; // Out of memory!
}

void kfree_page(void* addr, size_t size) {
    uint64_t phys_addr = (uint64_t)addr;
	if (phys_addr % 4096 != 0) { /* Panic */ }
    uint64_t pages_to_free = (size + 4095) / 4096;

    for (uint64_t i = 0; i < pages_to_free; i++) {
        uint64_t current_addr = phys_addr + (i * 4096);
        pmm_free_page(current_addr);
    }
}

uint64_t get_memupper() {return mem_upper;}