#include "array.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"



void arrayInit(Array* array,size_t elemSize){
	array->cap = 0;
	array->count = 0;
	array->elemSize = elemSize;
	array->elems = NULL;
}


int arrayWrite(Array* array,void* value){
	if (array->count + 1 > array->cap){
		int newCap =  ARRAY_CAP_GROW(array->cap);
		array->elems = ARRAY_GROW(array,newCap);
		array->cap = newCap;
	}
	void* elem = array->elems + array->count*array->elemSize;
	memcpy(elem,value,array->elemSize);
	array->count++;
	return array->count - 1;
}

void* arrayGet(Array* array,int index){
	if (index >= array->count){
		fprintf(stderr,"panic: index out of range");
		exit(64);
	}
	return array->elems + array->elemSize*index;
}

void arrayIterator(Array* array,void(*f)(void* arg)){
	for (int i = 0;i<array->count;i++){
		f(ARRAY_INDEX(array,i));
	}
}

void arrayFree(Array* array){
	freeMemory(array->elems);
	array->cap = 0;
	array->count= 0;
	array->elemSize=0;
}

int arrayGetCount(Array* array){
	return array->count;
}
