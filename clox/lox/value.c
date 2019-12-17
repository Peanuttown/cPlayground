#include "value.h"
#include "common.h"
#include "memory.h"
#include <stdio.h>
#include "object.h"
#include <stdlib.h>
#include <string.h>

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

static bool objectsEqual(Obj* a,Obj* b){
	if (a->type != b->type){
		return false;
	}
	ObjType objType = a->type;
	switch (objType){
		case OBJ_STRING:{
					ObjString* stringA = (ObjString*)a;
					ObjString* stringB = (ObjString*)b;
					if (stringA->length == stringB->length && \
							memcmp(stringA->chars,stringB->chars,stringA->length)==0\
							){
						return true;
					}
					return false;
				}
		default :{
				 fprintf(stderr,"objectsEqual error: unexpected obj type %d\n",a->type);
				 exit(64);
			 }
	}


}

bool valuesEqual(Value a,Value b){
	if (a.type != b.type) return false;
	switch(a.type){
		case VAL_BOOL:return AS_BOOL(a) == AS_BOOL(b);
		case VAL_NIL:return true;
		case VAL_NUMBER:return AS_NUMBER(a) == AS_NUMBER(b);
		case VAL_OBJ:return objectsEqual(AS_OBJ(a),AS_OBJ(b));
		default:{
				fprintf(stderr,"valuesEqual error,Unexpected value type %d\n",a.type);
				exit(64);
			}
	}
}

static void printObj(Obj* obj){
	switch(obj->type){
		case OBJ_STRING:{
					ObjString* string = (ObjString*)obj;
					printf("%.*s",string->length,string->chars);
					break;
				}
		default:{
				fprintf(stderr,"printObj error,unexpected obj type %d\n",obj->type);
				exit(64);
			}
	}
}

void printValue(Value value){
	if (IS_NUMBER(value)){
		printf("%g",AS_NUMBER(value));
	}else if(IS_BOOL(value)){
		printf(AS_BOOL(value)?"true":"false");
	}else if (IS_NIL(value)){
		printf("NULL");
	}else if(IS_OBJ(value)){
		printObj(AS_OBJ(value));
	}else{
		printf("printValue error :unexpected value type %d\n",value.type);
	}
}
