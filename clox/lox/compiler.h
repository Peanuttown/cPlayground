#ifndef lox_compiler_h
#define lox_compiler_h

#include "common.h"
#include "chunk.h"
#include "object.h"

ObjFunction* compile(char* source);


#endif
