#include "memory.h"
#include "stdlib.h"

void* reallocate(void* ptr,size_t size){
	if (size ==0){
		free(ptr);
		return NULL;
	} 
	return realloc(ptr,size);
}
