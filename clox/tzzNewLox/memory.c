#include "memory.h"
#include <stdlib.h>
#include "string.h"

void *allocateMemory(void* oldPtr,size_t newSize){
	return realloc(oldPtr,newSize);
}

void freeMemory(void* ptr){
	free(ptr);
}
void* copyCString(char* str,int length){
	char* ptr =alloca(sizeof(char)*(length+1));
	ptr[length] ='\0';
	memcpy(ptr,str,length);
	return ptr;
}

