#pragma once

#define INTERNAL_CLOCK_RUN_RATE 1193182

typedef unsigned int uint32_t;

inline uint32_t sec_to_ms(unsigned int seconds) {return seconds * 1000;}
void pit_init(uint32_t frequency);
void pit_handler();
void sleep(uint32_t time_in_miliseconds);