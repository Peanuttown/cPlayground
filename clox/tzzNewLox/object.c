#include "object.h"
#include <stdio.h>
#include "memory.h"
#include <string.h>
#include "chunk.h"
#include <math.h>
#include "table.h"
#include <stdlib.h>
#include "value.h"
#include "vm.h"

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

static ObjString* allocateString(VM* vm,char* chars,int length,int hash){
	ObjString* obj = (ObjString*)ALLOCATE_OBJ(ObjString,OBJ_STRING);
	obj->chars = chars;
	obj->length =length;
	obj->hash = hash;
	tableSet(&vm->strings,obj,NIL_VAL);
	printf("alloctString,%p,%.*s\n",obj,length,chars );
	return obj;
}

ObjString* copyString(VM* vm,const char* chars,int length){
	//find a interning string
	uint32_t hash = hashString(chars,length);
	ObjString* interned = tableFindString(&vm->strings,chars,length,hash);
	if (interned !=NULL) return interned;

	char* heapChars = ALLOCATE(char,length+1);
	memcpy(heapChars,chars,length);
	heapChars[length] = '\0';

	return allocateString(vm,heapChars,length,hash);
}

ObjString* takeString(VM* vm,char* chars,int length){
	int hash = hashString(chars,length);
	ObjString* interned = tableFindString(&vm->strings,chars,length,hash);
	if (interned != NULL){
		freeMemory(chars);
		return interned;
	}
	return allocateString(vm,chars,length,hash);
}
