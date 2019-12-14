#include "memory.h"
#include <stdlib.h>
#include <stdio.h>

void* allocMemory(size_t size){
	if (size ==0 ){
		return NULL;
	}
	return malloc(size);
}

void* reallocMemory(void* srcPtr,size_t newSize){
	return realloc(srcPtr,newSize);
}

void freeMemory(void* ptr){
	if (ptr != NULL){
		free(ptr);
	}
}
