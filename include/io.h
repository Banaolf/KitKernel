#pragma once
extern "C" {
    #include <stdint.h>
    // Write a byte to a port
    inline void outb(uint16_t port, uint8_t val) {
        asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    }

    // Read a byte from a port
    inline uint8_t inb(uint16_t port) {
        uint8_t ret;
        asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
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