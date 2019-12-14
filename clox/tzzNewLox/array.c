#include "array.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void initArray(Array* array,size_t elemSize){
	array->count =0;
	array->capacity=0;
	array->values=NULL;
	array->elemSize=elemSize;
}

int writeArray(Array* array,void* elem){
	if (array->capacity < array->count + 1){
		array->capacity = GROW_CAPACITY(array->capacity);
		array->values = reallocMemory(array->values,array->elemSize*array->capacity);
	}
	memcpy(array->values+array->count*array->elemSize,elem,array->elemSize);
	array->count++;
	return array->count-1;
}

void* getArray(Array* array,int index){
	return array->values+index*array->elemSize;
}

void freeArray(Array* array){
	array->count=0;
	array->capacity=0;
	free(array->values);
}

void writeArrayAt(Array *array,void *elem,int index){
	if (index >= array->count){
		fprintf(stderr,"cannont write after count\n");
		exit(64);
	}
	memcpy(array->values+array->elemSize*index,elem,array->elemSize);
}
