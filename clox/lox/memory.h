#ifndef lox_memory_h
#define lox_memory_h

#include "object.h"
#include "common.h"
#define GROW_CAPACITY(cap) \
	((cap)<8?8:(cap)*2);


#define GROW_ARRAY(ptr,type,oldCap,newCap) \
	(type*)(reallocate(ptr,sizeof(type)*oldCap,sizeof(type)*newCap))

#define FREE_ARRAY(type,ptr,oldCap) \
	reallocate(ptr,sizeof(type)*oldCap,0);


#define ALLOCATE(type,size) \
	(type*)reallocate(NULL,0,sizeof(type)*size)

#define FREE(type,ptr) \
	reallocate(ptr,sizeof(type),0)

void* reallocate(void* ptr,size_t oldSize,size_t newSize);
void freeObject(Obj* obj);
#endif
