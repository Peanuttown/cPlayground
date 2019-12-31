#ifndef lox_object_h
#define lox_object_h

#include "chunk.h"
#include "vm.h"
#include "value.h"

#define ALLOCATE_OBJ(obj,objType) \
	allocalObj(sizeof(obj),objType)


typedef enum{
	OBJ_STRING,
	OBJ_FUNCTION,
}ObjType;

struct sObj{
	ObjType type;
};

typedef struct{
	Obj obj;
	Chunk chunk;
	int arity;
}ObjFunction;

struct sObjString{
	Obj obj;
	int length;
	char* chars;
	int hash;
};

ObjFunction* newFunction();

Obj* allocalObj(size_t size,ObjType type);
ObjString* copyString(VM* vm,const char* chars,int length);


#endif
