#ifndef lox_object_h
#define lox_object_h

#include "chunk.h"
#include "vm.h"
#include "table.h"
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

struct sObjFunction {
	Obj obj;
	Chunk chunk;
	int arity;
};

struct sObjString{
	Obj obj;
	int length;
	char* chars;
	int hash;
};

struct sObjFunction* newFunction();

Obj* allocalObj(size_t size,ObjType type);
struct sObjString* copyString(Table* strings,const char* chars,int length);
struct sObjString* takeString(Table* strings,char* chars,int length);


#endif
