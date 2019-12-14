#ifndef lox_array_h
#define lox_array_h

#include "common.h"
#define InitArray(array,type)\
	initArray(array,sizeof(type))

#define GROW_CAPACITY(capacity)\
	(capacity<8?8:capacity*2)


typedef struct{
	int count;
	int capacity;
	void* values;
	size_t elemSize;
}Array;

void initArray(Array* array,size_t elemSize);

int writeArray(Array* array,void* elem);

void writeArrayAt(Array* array,void *elem,int index);

void* getArray(Array* array,int index);




#endif
