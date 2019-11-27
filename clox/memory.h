#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"

#define GROW_ARRAY(pointer,type,count)\
	(type*)reallocate(pointer,sizeof(type)*count)

#define GROW_CAPACITY(oldCapacity) \
	((oldCapacity)<8 ?8:(oldCapacity)*2)

#define FREE_ARRAY(ptr,type,count)\
	reallocate(ptr,sizeof(type)*count)

void* reallocate(void* pointer,size_t size);
#endif
