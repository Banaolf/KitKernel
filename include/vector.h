#pragma once

#include <stddef.h>
#include <stdint.h>
#include "heap.h"

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
    //Push an object into the array. O(1) + reallocating
    void push(Type x) {
        if (childcount == 0) {this->arr[0] = x; this->childcount++; this->size = sizeof(Type); return;} //If first object after init
        if ((sizeof(Type) * (this->childcount+1)) > this->size) {
            this->size *= 2;
            Type* arrsnapshot = this->arr;
            this->arr = (Type*)krealloc(this->arr, this->size);
            if (!this->arr) {this->bits |= (1 << 7); this->arr = arrsnapshot; this->size /= 2;}
            else if (this->arr == arrsnapshot) {this->bits |= (1 << 6);}
            this->arr[this->childcount++] = x;
        } else {
            this->arr[this->childcount++] = x;
        }
    }
    //Pop an item from the array (get and delete) O(1)
    void pop(Type* tpt) {
        if (this->childcount == 0) {this->bits |= (uint8_t)BitMeaning::Empty; return;}
        this->childcount--;
        *tpt = this->arr[this->childcount];
        if (this->size/2 > (sizeof(Type) * this->childcount)) {
            size /= 2;
            Type* arrsnapshot = this->arr;
            this->arr = (Type*)krealloc(this->arr, this->size);
            if (!this->arr) {this->bits |= (1 << 7); this->arr = arrsnapshot; this->size *= 2;}
            else if (this->arr == arrsnapshot) {this->bits |= (1 << 6);}
        }
    }
    //Get an item from the array O(1)
    Type* get() {
        if (this->childcount == 0) {this->bits |= (uint8_t)BitMeaning::Empty; return nullptr;}
        return this->arr[this->childcount-1];
    }
    //Get an item from the array at a specific index O(1)
    Type* getAt(int __idx) {
        if (this->childcount == 0) {this->bits |= (uint8_t)BitMeaning::Empty; return nullptr;}
        if (__idx >= this->childcount) return &this->arr[this->childcount-1];
        else if (__idx < 0) return &this->arr[0];
        return &this->arr[__idx];
    }
    //Get the byte size of the vector
    size_t getSize() {
        return this->size;
    }
    //Get the amount of children the vector has.
    int getLength() {
        return this->childcount;
    }
    //The meanings of bits
    enum class BitMeaning {
		Bad = (1 << 7), MaxMem = (1 << 6), Empty=(1<<5), RESERVED2=(1<<4), RESERVED3=(1<<3), RESERVED4=(1<<2), RESERVED5=(1<<1), RESERVED6=(1<<0)
	};
    //Checks if it has x bit.
    bool has_bit(BitMeaning bit) {
        return this->bits & (uint8_t)bit;
    }
};