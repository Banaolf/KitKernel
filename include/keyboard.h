#pragma once
#include <stdint.h>

//Shift keys 80-81
#define LSHIFT 0x80
#define RSHIFT 0x81
//Control Keys 82-83
#define LCTRL 0x82
#define RCTRL 0x83
//Arow keys 84-87
#define ARROWUP 0x84
#define ARROWRIGHT 0x85
#define ARROWDOWN 0x86
#define ARROWLEFT 0x87
//Alt keys 88-89
#define LALT 0x88
#define RALT 0x89
//Escape key 8A
#define ESC 0x8A
//Function keys (f keys) 8B-95
#define FUNCTION1 0x8B
#define FUNCTION2 0x8C
#define FUNCTION3 0x8D
#define FUNCTION4 0x8E
#define FUNCTION5 0x8F
#define FUNCTION6 0x90
#define FUNCTION7 0x91
#define FUNCTION8 0x92
#define FUNCTION9 0x93
#define FUNCTION10 0x94
#define FUNCTION11 0x95
#define FUNCTION12 0x96
//Front Keys (Start, End, Insert, Delete, etc) 97-9C
#define STARTKEY 0x97
#define ENDKEY 0x98
#define DELETEKEY 0x99
#define INSERTKEY 0x9A
#define PDOWN 0x9B
#define PUP 0x9C
//Misc (other) 9D-...
#define LSUPER 0x9D
#define RSUPER 0x9E
#define CAPSLOCK 0x9F
#define NUMLOCK 0xA0

extern volatile bool newlined;
extern bool khas_interrupted;

void keyboard_init();
uint8_t getModifiers();
void keyboard_handler_callback();