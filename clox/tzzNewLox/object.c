#include "value.h"
#include "object.h"
#include "chunk.h"

ObjFunction* newFunction(){
	ObjFunction* func=ALLOCATE_OBJ(ObjFunction,OBJ_FUNC);
	func->arity =0;
	func->name=NULL;
	initChunk(&func->chunk);
	return func;
}






