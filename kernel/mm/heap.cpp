#include "../../include/heap.h"
#include "../../include/pmm.h"
#include <stdint.h>

void map_page(uint64_t pml4_phys, uint64_t virt, uint64_t phys, uint64_t flags) {
    page_table* current_table = (page_table*)pml4_phys;
    
    uint64_t indices[4] = { 
        pml4_idx(virt), 
        pdpt_idx(virt), 
        pd_idx(virt), 
        pt_idx(virt) 
    };

    for (int i = 0; i < 3; i++) {
        uint64_t entry = current_table->entries[indices[i]];

        if (!(entry & PRESENT)) {
            uint64_t new_table = (uint64_t)kmalloc_page(4096);
            zero_page((void*)new_table);

            current_table->entries[indices[i]] = new_table | PRESENT | WRITABLE;
            current_table = (page_table*)new_table;
        } else {
            current_table = (page_table*)get_addr(entry);
        }
    }

    current_table->entries[indices[3]] = phys | flags | PRESENT;
    
    invlpg(virt);
}

void paging_init() {
    uint64_t* pml4 = (uint64_t*)kmalloc_page(4096);
    zero_page(pml4);

    for (uint64_t addr = 0; addr < (4 * 1024 * 1024); addr += 4096) {
        map_page((uint64_t)pml4, addr, addr, WRITABLE);
    }

    load_cr3((uint64_t)pml4);
}

void heap_init(uint64_t pml4_phys) {
    uint64_t heap_virtual_start = 0x100000000; 

    for (uint64_t i = 0; i < 1024 * 1024; i += 4096) {
        void* phys = kmalloc_page(4096);
        
        map_page(pml4_phys, heap_virtual_start + i, (uint64_t)phys, WRITABLE);
    }
}