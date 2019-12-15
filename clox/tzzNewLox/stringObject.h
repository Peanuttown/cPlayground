#ifndef lox_string_h
#define lox_string_h

#include "value.h"

struct sObjString{
	Obj obj;
	char* str;
	int length;
	uint32_t hash;
};

void initsObjString(struct sObjString* objStr);

struct sObjString* copyString(char* source,int length);

struct sObjString* allocObjString(char* heapChar,int length,int hash);

#endif
