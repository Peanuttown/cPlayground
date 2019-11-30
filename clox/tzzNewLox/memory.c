#include "memory.h"

#include <stdlib.h>

void* allocMemory(size_t size){
	return malloc(size);
}

void* reallocMemory(void* srcPtr,size_t newSize){
	return realloc(srcPtr,newSize);
}

void freeMemory(void* ptr){
	free(ptr);
}
