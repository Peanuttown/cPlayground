#include "value.h"
#include "common.h"
#include "memory.h"
#include <stdio.h>

void initValueArray(ValueArray* array){
	array->count = 0;
	array->cap = 0;
	array->values = NULL;
}

void writeValueArray(ValueArray* array,Value value){
	if (array->count +1  >  array->cap){
		int newCap = GROW_CAPACITY(array->cap);
		array->values = GROW_ARRAY(array->values,Value,array->cap,newCap);
		array->cap = newCap;
	}
	array->values[array->count] = value;
	array->count++;
}

void freeValueArray(ValueArray* array){
	FREE_ARRAY(Value,array->values,array->cap);
	initValueArray(array);
}

void printValue(Value value){
	printf("%g",value);
}
