#include "array.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>


void initArray(Array* array,size_t elemSize){
	array->count =0;
	array->capacity=0;
	array->values=NULL;
	array->elemSize=elemSize;
}

void writeArray(Array* array,void* elem){
	if (array->capacity < array->count + 1){
		array->capacity = GROW_CAPACITY(array->capacity);
		array->values = reallocMemory(array->values,array->elemSize*array->capacity);
	}
	memcpy(array->values+array->count*array->elemSize,elem,array->elemSize);
	array->count++;
}

void* getArray(Array* array,int index){
	return array->values+index*array->elemSize;
}

void freeArray(Array* array){
	array->count=0;
	array->capacity=0;
	free(array->values);
}
