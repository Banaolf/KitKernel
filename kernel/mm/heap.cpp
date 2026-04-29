#include "../../include/heap.h"
#include "../../include/out.h"
#include "../../include/pmm.h"
#include <stdint.h>
#include <stddef.h>

uint64_t g_pml4_phys = 0;
Mhdr_t* head = (MemoryHeader*)HEAPVSTART;
Mhdr_t* current = head;

void map_page(uint64_t pml4_phys, uint64_t virt, uint64_t phys, uint64_t flags) {
    page_table* current_table = (page_table*)pml4_phys;
    uint64_t indices[4] = { pml4_idx(virt), pdpt_idx(virt), pd_idx(virt), pt_idx(virt) };

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
    serial_print("Allocating pml4...\n");
    uint64_t* local_pml4 = (uint64_t*)kmalloc_page(4096);
    
    if (!local_pml4) {
        serial_print("Pml4 couldn't be allocated!\n");
        while(1) { asm volatile("hlt"); }
    }

    serial_print("Zeroing pml4\n");
    zero_page(local_pml4);

    serial_print("Getting upper memory\n");
    uint64_t upper = get_memupper();
    serial_print("Mapping PML4\n");
    for (uint64_t addr = 0; addr < upper; addr += 4096) {
        map_page((uint64_t)local_pml4, addr, addr, WRITABLE);
    }

    serial_print("Assigning g_mpl4_phys\n");
    g_pml4_phys = (uint64_t)local_pml4;
    
    serial_print("Making final assembly stub\n");
    asm volatile("mov %0, %%cr3" : : "r"(g_pml4_phys) : "memory");
}

void heap_init(uint64_t pml4_phys) {
    serial_print("Mapping pages...\n");
    #define __HEAP__INIT__ 0
    for (uint64_t i = 0; i < HEAPSIZE; i += 4096) {
        serial_print("Allocating page...\n");
        void* phys = kmalloc_page(4096);
        map_page(pml4_phys, HEAPVSTART + i, (uint64_t)phys, WRITABLE);
    }
    serial_print("Heap initiated!\n");
    has_heap_initted = true;

    head->size = HEAPSIZE - sizeof(MemoryHeader);
    head->is_free = true;
    head->next = nullptr;
    head->prev = nullptr;

    serial_print("Created the master Memory Header. Heap is now ready.\n");
}

void* kmalloc(size_t size) {
    size = (size + 15) & ~15;
    Mhdr_t* start_point = current;

    do {
        if (current->is_free && current->size >= size) {
            
            if (current->size >= (size + sizeof(Mhdr_t) + MIN_ALLOC_SIZE)) {
                Mhdr_t* new_next = (Mhdr_t*)((uint8_t*)current + sizeof(Mhdr_t) + size);
                new_next->size = current->size - size - sizeof(Mhdr_t);
                new_next->is_free = true;
                new_next->next = current->next;
                new_next->prev = current;

                if (new_next->next) new_next->next->prev = new_next;

                current->size = size;
                current->next = new_next;
            }

            current->is_free = false;
            void* retv = (void*)((uint8_t*)current + sizeof(Mhdr_t));

            current = (current->next) ? current->next : head;
            return retv;
        }

        current = (current->next) ? current->next : head;

    } while (current != start_point);

    serial_print("Out of heap memory!\n");
    return nullptr;
}

static void coalescemhdrs(Mhdr_t*& hdr, bool backwards=false) {
    if (!backwards) {
        hdr->size += hdr->next->size + sizeof(MemoryHeader);
        if (hdr->next->next) {hdr->next->next->prev = hdr;}
        hdr->next = hdr->next->next;
    } else {
        hdr = hdr->prev;
        hdr->size += hdr->next->size + sizeof(MemoryHeader);
        if (hdr->next->next) {hdr->next->next->prev = hdr;}
        hdr->next = hdr->next->next;
    }
}

void kfree(void* ptr) {
    if (ptr == nullptr) return;
    bool is_standing_on_current = false;
    Mhdr_t* hdr = (Mhdr_t*)((uint8_t*)ptr - sizeof(Mhdr_t));
    if (hdr == current) is_standing_on_current = true;
    hdr->is_free = true;

    while (hdr->next != nullptr && hdr->next->is_free) {
        coalescemhdrs(hdr);
    }
    while (hdr->prev != nullptr && hdr->prev->is_free) {
        coalescemhdrs(hdr, true);
    }
    
    if (is_standing_on_current) current = hdr;

    serial_print("Memory coalesced and freed.\n");
}

void kpzero(void* ptr) {
    if (ptr == nullptr) return;

    Mhdr_t* hdr = (Mhdr_t*)((uint8_t*)ptr - sizeof(Mhdr_t));
    uint8_t* dest = (uint8_t*)ptr;

    for (size_t i = 0; i < hdr->size; i++) {
        dest[i] = 0;
    }
}

void* kcalloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    void* ptr = kmalloc(total);

    if (ptr != nullptr) kpzero(ptr);

    return ptr;
}

void kmcopy(void *__src, void *__dst, const size_t size) {
    uint8_t* src = (uint8_t*)__src;
    uint8_t* dst = (uint8_t*)__dst;
    if (src < dst + size && dst < src + size) {serial_print("(kmcopy)Pointers cannot overlap!\n"); return;}

    for (size_t i = 0; i < size; i++) {
        dst[i] = src[i];
    }
}

void* krealloc(void* __ptr, size_t size) {
    if (!__ptr) return NULL;
    size = (size + 15) & ~15;

    Mhdr_t* hdr = (Mhdr_t*)((uint8_t*)__ptr - sizeof(MemoryHeader));
    size_t old_size = hdr->size;

    if (hdr->next && hdr->next->is_free && (hdr->size + sizeof(MemoryHeader) + hdr->next->size) >= size) {
        coalescemhdrs(hdr);
        serial_print("Reallocating memory: Next.\n");
        return __ptr; 
    } else if (hdr->prev && hdr->prev->is_free && (hdr->size + sizeof(MemoryHeader) + hdr->prev->size) >= size) {
        kmcopy((void*)((uint8_t*)hdr + sizeof(MemoryHeader)), 
               (void*)((uint8_t*)hdr->prev + sizeof(MemoryHeader)), 
               (old_size > size) ? size : old_size);
        
        coalescemhdrs(hdr, true);

        serial_print("Reallocating memory: Prev.\n");
        return (void*)((uint8_t*)hdr + sizeof(MemoryHeader));
    }
    else if ((hdr->prev && hdr->prev->is_free) && (hdr->next && hdr->next->is_free) && 
            (hdr->prev->size + hdr->size + hdr->next->size + (2 * sizeof(MemoryHeader))) >= size) {
        
        coalescemhdrs(hdr);
        kmcopy((void*)((uint8_t*)hdr + sizeof(MemoryHeader)), 
               (void*)((uint8_t*)hdr->prev + sizeof(MemoryHeader)), 
               (old_size > size) ? size : old_size);
        
        coalescemhdrs(hdr, true);

        serial_print("Reallocating memory: Next & Prev.\n");
        return (void*)((uint8_t*)hdr + sizeof(MemoryHeader));
    }
    else {
        void* ptr = kmalloc(size);
        if (!ptr) {
            serial_print("Found no place for new memory (krealloc)\n");
            return NULL; 
        }

        kmcopy((void*)((uint8_t*)hdr + sizeof(MemoryHeader)), ptr, (old_size > size) ? size : old_size);
        
        kfree((void*)((uint8_t*)hdr + sizeof(MemoryHeader)));

        serial_print("Reallocating memory: Allocated new block.\n");
        return ptr;
    }
}