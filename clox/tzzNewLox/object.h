#ifndef lox_object_h
#define lox_object_h
#include "value.h"
#include "stringObject.h"
#include "chunk.h"

typedef struct {
	Obj obj;
	ObjString* name;
	int arity;
	Chunk chunk;
}ObjFunction;

ObjFunction* newFunction();


#endif
