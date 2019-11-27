#include "value.h"
#include "common.h"
#include "memory.h"



void initValueArray(ValueArray* array){
	array->count =0;
	array->capacity =0;
	array->values=NULL;
}

int writeValueArray(ValueArray* array,Value value){
	if (array->capacity < array->count+1){
		array->capacity = GROW_CAPACITY(array->capacity);
		array->values = realloc(array->values,sizeof(Value)*array->capacity);
	}
	array->values[array->count] = value;
	array->count++;
	return array->count-1;
}

void freeValueArray(ValueArray* array){
	free(array->values);
	array->count =0;
	array->capacity =0;
}
