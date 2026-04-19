#include "../../include/pmm.h"
#include <stdint.h>

static struct bitmap Bitmap;

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

void pmm_init(uint64_t* multiboot_addr) {
	multiboot_tag* tag = (multiboot_tag*)(multiboot_addr + 8);
	uint64_t mem_upper = 0;
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
}