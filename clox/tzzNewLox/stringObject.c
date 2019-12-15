#include "stringObject.h"
#include "string.h"
#include "memory.h"
#include "value.h"
#include "vm.h"
#include <stdio.h>

static uint32_t hash(char* key,int length){
	uint32_t hash = 2166136261u;

	for (int i = 0; i < length; i++) {                     
		hash ^= key[i];                                      
		hash *= 16777619;                                    
	}                                                      

	return hash;
}

void initsObjString(struct sObjString* objStr){
	objStr->obj.type = OBJ_STRING;
}

struct sObjString* tableFindString(Table* table,char* chars,int length,uint32_t hash){
	if (table->count ==0){
		return NULL;
	}
	int index = hash % table->capacity;
	if (index < 0){
		index = -index;
	}
	for(;;){
		Entry entry = table->entry[index];
		if (entry.key == NULL){
			return NULL;
		}
		if ((entry.key->length == length)&&(memcmp(entry.key->str,chars,length)==0)){
			return entry.key;
		}
		index = (index+1) % table->capacity;
	}
}

struct sObjString* copyString(char* chars,int length){
	int hashValue = hash(chars,length);
	struct sObjString* interned = tableFindString(&vm.strings,chars,length,hashValue);
	if (interned != NULL){
		return interned;
	}
	char* heapChar = allocMemory(sizeof(char)*(length+1));
	memcpy(heapChar,chars,length);
	heapChar[length] = '\0';
	return allocObjString(heapChar,length,hashValue);
}


struct sObjString* allocObjString(char* heapChar,int length,int hash){
	struct sObjString*  objString = ALLOCATE_OBJ(ObjString,OBJ_STRING);
	objString->length = length;
	objString->str = heapChar;
	objString->hash = hash;
	TableSet(&vm.strings,objString,&AS_NIL);
	return objString;
}
