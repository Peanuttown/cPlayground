#ifndef lox_vm_h
#define lox_vm_h
#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct{
	Chunk* chunk;
	uint8_t* ip;
	Value stack[STACK_MAX];
	Value* stackTop;
}VM;

typedef enum{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERR,
	INTERPRET_RUNTIME_ERR,

}InterpretResult;

InterpretResult interpret(char* source);

void initVM();
void freeVM();
void push(Value value);
Value pop();

#endif