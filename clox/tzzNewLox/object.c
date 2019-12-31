#include "object.h"
#include <stdio.h>
#include "memory.h"
#include <string.h>
#include "chunk.h"
#include <math.h>
#include <stdlib.h>
#include "value.h"
#include "vm.h"
#include "table.h"

Obj* allocalObj(size_t size,ObjType type){
	Obj* obj =allocateMemory(NULL,size);
	obj->type = type;
	return obj;
}

ObjFunction* newFunction(){
	ObjFunction* function =(ObjFunction*)ALLOCATE_OBJ(ObjFunction,OBJ_FUNCTION);
	function->arity =0;
	chunkInit(&function->chunk);
	return function;
}

static int hashString(const char* key,int length){
	int hash= 564321;
	for (int i =0;i<length;i++){
		hash ^= key[i];
		hash *=165348;
	}
	hash = abs(hash);
	return hash;

}

static ObjString* allocateString(Table* strings,char* chars,int length,int hash){
	ObjString* obj = (ObjString*)ALLOCATE_OBJ(ObjString,OBJ_STRING);
	obj->chars = chars;
	obj->length =length;
	obj->hash = hash;
	tableSet(strings,obj,NIL_VAL);
	return obj;
}

ObjString* copyString(Table* strings,const char* chars,int length){
	//find a interning string
	uint32_t hash = hashString(chars,length);
	ObjString* interned = tableFindString(strings,chars,length,hash);
	if (interned !=NULL) return interned;

	char* heapChars = ALLOCATE(char,length+1);
	memcpy(heapChars,chars,length);
	heapChars[length] = '\0';

	return allocateString(strings,heapChars,length,hash);
}

ObjString* takeString(Table* strings,char* chars,int length){
	int hash = hashString(chars,length);
	ObjString* interned = tableFindString(strings,chars,length,hash);
	if (interned != NULL){
		freeMemory(chars);
		return interned;
	}
	return allocateString(strings,chars,length,hash);
}
