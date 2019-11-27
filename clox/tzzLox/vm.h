#ifndef lox_vm_h
#define lox_vm_h

#define STACK_MAX 256

#include "chunk.h"
typedef struct{
	Chunk* chunk;
	uint8_t* ip; //instruction pointer
	Value stack[STACK_MAX];
	Value* stackTop;
}VM;

typedef enum{
	INTERPRETER_RESULT_OK,
	INTERPRETER_RESULT_RUNTIME_ERR,
	INTERPRETER_RESULT_COMPILE_ERR,

}InterpreterResult;

InterpreterResult interpret(Chunk* chunk);

void runFile(char* filename);

#endif
