#ifndef lox_vm_h
#define lox_vm_h

#include "object.h"
#include "common.h"
#include "value.h"
#include "table.h"

#define FRAME_MAX 64
#define STACK_MAX (FRAME_MAX*UINT8_MAX)

typedef enum{
	INTERPRET_OK,
	INTERPRET_COMPILE_ERR,
	INTERPRET_RUNTIME_ERR,
}InterpretResult;



typedef struct{
	uint8_t* ip;
	ObjFunction* function;
	Value* slots;
}CallFrame;

typedef struct{
	CallFrame callFrames[FRAME_MAX];
	int frameCount;
	Value stack[STACK_MAX];
	Value* stackTop;
	Table strings;
}VM;

void vmInit(VM* vm);
void vmFree(VM* vm);
void push(VM* vm,Value value);
Value pop(VM* vm);

InterpretResult interpret(char* source);



#endif
