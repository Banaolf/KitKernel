#pragma once
extern "C" {
    #include <stdint.h>
    
    // 8-bit write
    inline void outb(uint16_t port, uint8_t val) {
        asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
    }

    // 8-bit read
    inline uint8_t inb(uint16_t port) {
        uint8_t ret;
        asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
    }

    // 16-bit write (FIXED: changed val to uint16_t)
    inline void outw(uint16_t port, uint16_t val) {
        asm volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
    }

    // 16-bit read
    inline uint16_t inw(uint16_t port) {
        uint16_t ret;
        asm volatile ("inw %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
    }

    // 32-bit write
    inline void outl(uint16_t port, uint32_t val) {
        asm volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
    }

    // 32-bit read
    inline uint32_t inl(uint16_t port) {
        uint32_t ret;
        asm volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
        return ret;
    }

    inline void halt() {
        asm volatile("cli");
        while (1) {asm volatile("hlt");}
    }

    // A small delay often needed for older hardware (like the PIC)
    [[deprecated]]
    inline void io_wait() {
        outb(0x80, 0); // Writing to an unused port
    }
}