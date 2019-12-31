#ifndef lox_array_h
#define lox_array_h

#include "memory.h"

#define ARRAY_INDEX(array,index) \
	(array)->elems+(array)->elemSize*index

#define ARRAY_WRITE(array,value,index,type) 

#define ARRAY_INIT(array,elemType) \
	arrayInit((array),sizeof(elemType))

#define ARRAY_CAP_GROW(oldCap) \
	((oldCap)<8?8:(oldCap)*2)
#define ARRAY_GROW(array,cap) \
	allocateMemory(array->elems,(cap)*array->elemSize)



#include "common.h"

typedef struct{
	int count;
	int cap;
	void* elems;
	size_t elemSize;
}Array;

void arrayInit(Array* array,size_t elemSize);
int arrayWrite(Array* array,void* value);
void* arrayGet(Array* array,int index);
void arrayIterator(Array* array,void(*f)(void* arg));
int arrayGetCount(Array* array);

void arrayFree(Array *array);



#endif
