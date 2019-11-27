#include "value.h"
#include "common.h"
#include "memory.h"

void initValueArray(ValueArray* array){
	array->count = 0;
	array->capacity =0;
	array->values = NULL;
}

void freeValueArray(ValueArray* array){
	FREE_ARRAY(array->values,Value,array->capacity);
	initValueArray(array);
}

void writeValueArray(ValueArray* array,Value value){
	//need expand capacity
	if (array->capacity < array->count + 1){
		array->capacity = GROW_CAPACITY(array->capacity);
		array->values= GROW_ARRAY(array->values,Value,array->capacity);
	}

	array->values[array->count] = value;
	array->count++;
}
