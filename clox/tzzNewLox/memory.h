#ifndef lox_memory_h
#define lox_memory_h

#include "common.h"

void* allocMemory(size_t size);

void* reallocMemory(void* srcPtr,size_t newSize);

void freeMemory(void* ptr);


#endif
