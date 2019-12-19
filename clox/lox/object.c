#include "object.h"
#include <stdlib.h>
#include "memory.h"
#include "value.h"
#include <string.h>
#include "vm.h"
#include "math.h"
#include "table.h"
#include <stdio.h>

#define ALLOCATE_OBJ(type,objType) \
	(type*)allocateObj(sizeof(type),objType)


static int hashString(const char* key,int length){
	int hash= 564321;
	for (int i =0;i<length;i++){
		hash ^= key[i];
		hash *=165348;
	}
	hash = abs(hash);
	return hash;

}

static Obj* allocateObj(size_t size,ObjType type){
	Obj* obj =  (Obj*)reallocate(NULL,0,size);
	obj->type = type;
	printf("obj type %d\n",type);
	obj->next =vm.objects;
	vm.objects = obj;
	return obj;
}

static ObjString* allocateString(char* chars,int length,int hash){
	ObjString* obj = ALLOCATE_OBJ(ObjString,OBJ_STRING);
	obj->chars = chars;
	obj->length =length;
	obj->hash = hash;
	tableSet(&vm.strings,obj,NIL_VAL);
	printf("alloctString,%p,%.*s\n",obj,length,chars );
	return obj;
}

ObjString* copyString(const char* chars,int length){
	//find a interning string
	uint32_t hash = hashString(chars,length);
	ObjString* interned = tableFindString(&vm.strings,chars,length,hash);
	if (interned !=NULL) return interned;

	char* heapChars = ALLOCATE(char,length+1);
	memcpy(heapChars,chars,length);
	heapChars[length] = '\0';

	return allocateString(heapChars,length,hash);
}

ObjString* takeString(char* chars,int length){
	int hash = hashString(chars,length);
	ObjString* interned = tableFindString(&vm.strings,chars,length,hash);
	if (interned != NULL){
		FREE_ARRAY(char,chars,length+1);
		return interned;
	}
	return allocateString(chars,length,hash);
}
