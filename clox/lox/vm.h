#ifndef lox_vm_h
#define lox_vm_h
#include "chunk.h"
#include "value.h"
#include "object.h"
#include "table.h"

#define STACK_MAX 256

typedef struct{
	Chunk* chunk;
	Value stack[STACK_MAX];
	Value* stackTop;
	Obj* objects;
	Table strings;//string interning
	Table globals;
	uint8_t* ip;
}VM;

extern VM vm;


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
