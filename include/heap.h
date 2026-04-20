#pragma once
#include <stdint.h>

inline uint64_t pml4_idx(uint64_t virt) { return (virt >> 39) & 0x1FF; }
inline uint64_t pdpt_idx(uint64_t virt) { return (virt >> 30) & 0x1FF; }
inline uint64_t pd_idx(uint64_t virt)   { return (virt >> 21) & 0x1FF; }
inline uint64_t pt_idx(uint64_t virt)   { return (virt >> 12) & 0x1FF; }

inline uint64_t get_addr(uint64_t entry) { return entry & 0x000FFFFFFFFFF000; }

inline bool is_present(uint64_t entry) { return entry & 1; }

inline void invlpg(uint64_t virt) {
    asm volatile("invlpg (%0)" : : "r"(virt) : "memory");
}

inline void load_cr3(uint64_t pml4_phys_addr) {
    asm volatile("mov %0, %%cr3" : : "r"(pml4_phys_addr) : "memory");
}

void heap_init(uint64_t pml4_phys);
void paging_init();