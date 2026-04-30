#pragma once

#include <stddef.h>
#include "heap.h"
typedef unsigned char uint8_t;

template<typename Type>
class Vector {
protected:
    Type* arr = nullptr;
    int childcount = 0;
    size_t size = 0;
    uint8_t bits = 0;
public:
    inline Vector() {this->arr = (Type*)kmalloc(sizeof(Type));};
    inline ~Vector() {kfree(this->arr); childcount = 0; size = 0;};
    void push(Type x);
    void pop(Type* tpt);
    Type get();
    Type getAt(int __idx);
    size_t getSize();
    int getLength();

    enum class BitMeaning {
		Bad = (1 << 7), MaxMem = (1 << 6), Empty=(1<<5), RESERVED2=(1<<4), RESERVED3=(1<<3), RESERVED4=(1<<2), RESERVED5=(1<<1), RESERVED6=(1<<0)
	};
    bool has_bit(Vector::BitMeaning bit);
};