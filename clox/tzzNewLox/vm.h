#ifndef lox_vm_h
#define lox_vm_h

#include "array.h"
#include "stack.h"
#include "value.h"
#include "table.h"

#define getCodeByIndex(index) \
	(uint8_t*)(getArray(&vm.code,index))

#define getConstantByIndex(index) \
	(Value*)(getArray(&vm.constants,index))


typedef struct{
	Array code;
	Array constants;
	Stack stack;
	int ip;
	Table strings;
	Table global;
}VM;

VM vm;

typedef enum{
	INTERPRETER_OK,
	INTERPRETER_FAILED,
	INTERPRETER_COMPILED_ERR,
	INTERPRETER_RUNTIME_ERR,
}InterpretResult;


void initVM();

InterpretResult compile();

InterpretResult runFile(char* filename);

#endif
