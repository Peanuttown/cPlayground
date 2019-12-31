#include "memory.h"
#include <stdlib.h>

void *allocateMemory(void* oldPtr,size_t newSize){
	return realloc(oldPtr,newSize);
}

void freeMemory(void* ptr){
	free(ptr);
}
