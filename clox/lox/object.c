#include "object.h"
#include <stdlib.h>
#include "memory.h"
#include "value.h"
#include <string.h>
#include "vm.h"

#define ALLOCATE_OBJ(type,objType) \
	(type*)allocateObj(sizeof(type),objType)

static Obj* allocateObj(size_t size,ObjType type){
	Obj* obj =  (Obj*)reallocate(NULL,0,size);
	obj->type = type;
	obj->next =vm.objects;
	vm.objects = obj;
	return obj;
}

static ObjString* allocateString(char* chars,int length){
	ObjString* obj = ALLOCATE_OBJ(ObjString,OBJ_STRING);
	obj->chars = chars;
	obj->length =length;

	return obj;
}

ObjString* copyString(const char* chars,int length){
	char* heapChars = ALLOCATE(char,length+1);
	memcpy(heapChars,chars,length);
	heapChars[length] = '\0';

	return allocateString(heapChars,length);
}

ObjString* takeString(char* chars,int length){
	return allocateString(chars,length);
}
