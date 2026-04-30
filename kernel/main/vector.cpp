#include "../../include/vector.h"
#include "../../include/heap.h"
#include "../../include/out.h"
#include <stddef.h>

#define self this

typedef unsigned char uint8_t;

	
//Push an object into the array. O(1) + reallocating
template<typename Type>
void Vector<Type>::push(Type x) {
	if (childcount == 0) {self->arr[0] = x; self->childcount++; self->size = sizeof(Type); return;} //If first object after init
	if ((sizeof(Type) * (self->childcount+1)) > self->size) {
		self->size *= 2;
		Type* arrsnapshot = self->arr;
		self->arr = (Type*)krealloc(self->arr, self->size);
		if (!self->arr) {serial_print("Array has turned to null!\n"); self->bits |= (1 << 7); self->arr = arrsnapshot; self->size /= 2;}
		else if (self->arr == arrsnapshot) {serial_print("Array is the same as before.\n"); self->bits |= (1 << 6);}
		self->arr[self->childcount++] = x;
	} else {
		self->arr[self->childcount++] = x;
	}
}

//Pop an item from the array (get and delete) O(1)
template<typename Type>
void Vector<Type>::pop(Type* tpt) {
	if (self->childcount == 0) {self->bits |= BitMeaning::Empty; return;}
	self->childcount--;
	*tpt = self->arr[self->childcount];
	if (self->size/2 > (sizeof(Type) * self->childcount)) {
		size /= 2;
		Type* arrsnapshot = self->arr;
		self->arr = (Type*)krealloc(self->arr, self->size);
		if (!self->arr) {serial_print("Array has turned to null!\n"); self->bits |= (1 << 7); self->arr = arrsnapshot; self->size *= 2;}
		else if (self->arr == arrsnapshot) {serial_print("Array is the same as before.\n"); self->bits |= (1 << 6);}
	}
}

//Get an item from the array O(1)
template<typename Type>
Type Vector<Type>::get() {
	if (self->childcount == 0) {self->bits |= BitMeaning::Empty; return;}
	return self->arr[self->childcount-1];
}

//Get an item from the array at a specific index O(1)
template<typename Type>
Type Vector<Type>::getAt(int __idx) {
	if (self->childcount == 0) {self->bits |= BitMeaning::Empty; return;}
	if (__idx >= self->childcount) return self->arr[self->childcount-1];
	else if (__idx < 0) return self->arr[0];
	return self->arr[__idx];
}

//Get the byte size of the vector
template<typename Type>
size_t Vector<Type>::getSize() {
	return self->size;
}

//Get the amount of children the vector has.
template<typename Type>
int Vector<Type>::getLength() {
	return self->childcount;
}

//Checks if it has x bit.
template<typename Type>
bool Vector<Type>::has_bit(BitMeaning bit) {
	return self->bits & bit;
}