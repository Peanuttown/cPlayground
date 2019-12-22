#include "memory.h"
#include <stdlib.h>
#include "object.h"
#include <stdio.h>

void *reallocate(void* ptr,size_t oldSize,size_t newSize){
	if (newSize == 0){
		free(ptr);
		return NULL;
	}

	return realloc(ptr,newSize);
}

void freeObject(Obj* obj){
	switch(obj->type){
		case OBJ_STRING:{
					ObjString* string = (ObjString*)obj;
					FREE_ARRAY(char,string->chars,string->length+1);
					FREE(ObjString,string);
					break;
				}

		case OBJ_FUNCTION:{
					  ObjFunction *func = (ObjFunction*)obj;
					  freeChunk(&func->chunk);
					  FREE(ObjFunction,func);
					  break;
				  }
		case OBJ_NATIVE:{
					FREE(ObjNative,obj);
					break;
				}
		case OBJ_CLOSURE:{
					 FREE(ObjClosure,obj);
					 break;
				 }
		default:{
				fprintf(stderr,"freeObject error : unexpect obj type %d\n",obj->type);
				exit(64);
			}

	}

}
