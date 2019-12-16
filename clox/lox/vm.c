#include "vm.h"
#include "chunk.h"
#include <stdio.h>
#include "debug.h"
#include "compiler.h"

VM vm;

static void resetStack(){
	vm.stackTop = vm.stack;
}

void push(Value value){
	*vm.stackTop = value;
	vm.stackTop++;
}

Value pop(){
	vm.stackTop--;
	return *vm.stackTop;
}


void initVM(){
	resetStack();
}

void freeVM(){

}

static InterpretResult run(){
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])


	for(;;){
#ifdef DEBUG_TRACE_EXECUTION
		//print stack value
		printf("     ");
		for(Value* slot = vm.stack;slot < vm.stackTop;slot++){
			printf("[");
			printValue(*slot);
			printf("]");
		}
		printf("\n");

		disassembleInstruction(vm.chunk,(int)(vm.ip - vm.chunk->code));
#endif
		uint8_t instruction;
		switch(instruction = READ_BYTE()){
			case OP_RETURN:{
					       printValue(pop());
					       return INTERPRET_OK;
				       }
			case OP_CONSTANT:{
						 Value value = READ_CONSTANT();
						 push(value);
						 break;
					 }
			default:{

				}
		}
	}
#undef READ_BYTE
#undef READ_CONSTANT
	return 0;
}

InterpretResult interpret(char* source){
	compile(source);
	return INTERPRET_OK;
}
