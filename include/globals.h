#pragma once
#define DEBUG_MODE 1
#include <stdint.h>
#ifdef __cplusplus
uint8_t get_ScancodeMapID();
void set_ScancodeMapID(uint8_t map);
void set_ScancodeMapID(int map);
uint8_t get_ScancodeMapSize();

const unsigned char* get_Scancode();
const unsigned char* get_Scancode_Shifted();
const unsigned char* get_Scancode_Altgr();
#endif
/*
0x00 = es_es
0x01 = es_la
0x02 = en_us
0x03 = en_uk
*/