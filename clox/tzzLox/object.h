#ifndef lox_object_h
#define lox_object_h

#include "value.h"
#include "memory.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)
#define IS_STRING(value) isObjType((value),OBJ_STRING)

#define AS_STRING(value) ((ObjString*)AS_OBJECT(value))
#define AS_CSTRING(value) ((AS_STRING(value))->chars)
#define ALLOC_OBJ(type,objType)\
	(type*)(allocObject(sizeof(type),objType))


typedef enum{
	OBJ_STRING,
}ObjType;

struct sObj{
	ObjType type;
};

struct sObjString {
	Obj obj;
	int length;
	char* chars;
};

static inline bool isObjType(Value value,ObjType type){
	return IS_OBJ(value) && OBJ_TYPE(value) == type;
}

Obj* allocObject(size_t size,ObjType type){
	Obj* obj = (Obj*)malloc(size);
	obj->type = type;
	return obj;
}


ObjString* newObjString(const char* chars,int length){
	char* heapChar = (char*)malloc(sizeof(char)*(length+1));
	memcpy(heapChar,chars,length);
	heapChar[length] ='\0';
	return ALLOC_OBJ(ObjString,OBJ_STRING);
}


#endif
