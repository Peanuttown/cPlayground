#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"


typedef struct{
	Chunk* chunk;
}VM;


typedef enum{
	INTERPRET_RESULT_OK,
	INTERPRET_RESULT_COMPILE_ERR,
	INTERPRET_RESULT_RUNTIME_ERR,

}InterpretResult;

void initVM();
void freeVM();

InterpretResult runFile(char* file);

#endif
