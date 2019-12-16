#ifndef lox_callframe_h
#define lox_callframe_h

#include "object.h"
#include "value.h"

typedef struct{
	ObjFunction* func;
	Value* slot;
	uint8_t ip;

}CallFrame;


#endif
