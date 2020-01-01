#ifndef lox_memory_h
#define lox_memory_h

#include "common.h"

#define ALLOCATE(elemType,elemNum) \
	allocateMemory(NULL,sizeof(elemType)*elemNum)
	

void* allocateMemory(void* oldPtr,size_t newAllocateSize);

void freeMemory(void* ptr);

void* copyCString(char* str,int length);


#endif
