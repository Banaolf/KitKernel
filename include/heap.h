#pragma once
#include <stdint.h>
#define HEAPVSTART 0x100000000
#define HEAPSIZE 512 * 512
#define MIN_ALLOC_SIZE 8

inline bool has_heap_initted = false;

inline uint64_t pml4_idx(uint64_t virt) { return (virt >> 39) & 0x1FF; }
inline uint64_t pdpt_idx(uint64_t virt) { return (virt >> 30) & 0x1FF; }
inline uint64_t pd_idx(uint64_t virt)   { return (virt >> 21) & 0x1FF; }
inline uint64_t pt_idx(uint64_t virt)   { return (virt >> 12) & 0x1FF; }

// Extract the physical address from an entry (clearing the flags)
inline uint64_t get_addr(uint64_t entry) { return entry & 0x000FFFFFFFFFF000; }

// Check if the present bit (Bit 0) is set
inline bool is_present(uint64_t entry) { return entry & 1; }

// Invalidate Page: Flushes the TLB for a specific virtual address
inline void invlpg(uint64_t virt) {
    asm volatile("invlpg (%0)" : : "r"(virt) : "memory");
}

// Loads the Page Map Level 4 into the CR3 register to activate paging
inline void load_cr3(uint64_t pml4_phys_addr) {
    asm volatile("mov %0, %%cr3" : : "r"(pml4_phys_addr) : "memory");
}

// The physical address of the root page table, used by the heap to map new pages
extern uint64_t g_pml4_phys;

/**
 * paging_init: Creates the initial PML4, identity maps the kernel and 
 * usable RAM, maps the framebuffer, and enables paging.
*/
void paging_init();

/**
 * heap_init: Sets up the virtual memory space for the kernel heap 
 * and initializes the first memory block headers.
*/
void heap_init(uint64_t pml4_phys);

/**
 * map_page: Maps a single 4KB virtual address to a physical address 
 * within the specified PML4 table.
*/
void map_page(uint64_t pml4_phys, uint64_t virt, uint64_t phys, uint64_t flags);

#include <stddef.h>

typedef struct MemoryHeader {
    size_t size;
    bool is_free;
    MemoryHeader* next;
    MemoryHeader* prev;
}__attribute__((aligned(16))) Mhdr_t;

/*
 *kmcopy: Copies data from a pointer to another. Does not check destination's values before overwriting. Only checks if the pointers overlap.
*/
void kmcopy(void *__src, void *__dst, const size_t size);

/**
 * kmalloc: Allocates x amount of space
 * into the memory and gives you the pointer to it
*/
void* kmalloc(size_t size);
/**
 * kfree: Frees x pointer from the memory
*/
void kfree(void* ptr);
/**
 * kzero: Zeroes out a pointer
*/
void kpzero(void* ptr);
/**
 * kcallor: Allocates a pointer full of zeroes
*/
void* kcalloc(size_t nmemb, size_t size);
/**
 * krealloc: Reallocates a pointer, with a new size and same data. Returns NULL if not succesfull.
*/
void* krealloc(void* __ptr, size_t size);