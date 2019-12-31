#ifndef lox_compile_h
#define lox_compile_h

#include "object.h"
#include "table.h"

ObjFunction* compile(char* source,Table* strings);

#endif
