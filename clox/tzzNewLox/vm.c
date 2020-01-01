#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include "compile.h"
#include "debug.h"
#include "table.h"

void vmInit(VM* vm){
	vm->stackTop = vm->stack;
	vm->frameCount = 0;
	initTable(&vm->strings);
	initTable(&vm-> global);
}

void vmFree(VM* vm){
	//todo
}

void push(VM* vm,Value value){
	*vm->stackTop = value;
	vm->stackTop++;
}

Value pop(VM* vm){
	if (vm->stackTop == vm->stack){
		fprintf(stderr,"vm stack out of index");
		exit(64);
	} 
	vm->stackTop--;
	return *vm->stackTop;
}



static CallFrame* getCurrentFrame(VM* vm){
	if (vm->frameCount==0){
		fprintf(stderr,"no current call frame\n");
		exit(64);
	}
	return &vm->callFrames[vm->frameCount-1];
}

static Value peek(VM* vm,int offset){
	return vm->stackTop[-1-offset];
}

static void setStack(VM* vm,int offset,Value value){
	getCurrentFrame(vm)->slots[offset] = value;
}
static Value getStack(VM* vm,int offset){
	return getCurrentFrame(vm)->slots[offset];
}

static bool call(VM* vm,ObjFunction* function,int argCount){
	if (function->arity != argCount){
		fprintf(stderr,"argCount not match");
		exit(64);
	}
	CallFrame* frame = &vm->callFrames[vm->frameCount++];
	frame->function = function;
	frame->ip = function->chunk.codes.elems;
	frame->slots = vm->stackTop - argCount - 1;
	return true;
}
static bool callValue(VM* vm,Value callee,int argCount){
	if (IS_OBJ(callee)){
		switch (OBJ_TYPE(callee)){
			case OBJ_FUNCTION:{
						 return call(vm,AS_FUNCTION(callee),argCount);
					 }
			default:{
					break;
				}

		}

	}
	fprintf(stderr,"Can only call functions and classes");
	exit(64);
	return false;
}


InterpretResult run(VM* vm){
	CallFrame* frame = getCurrentFrame(vm);

#define READ_BYTE() (*frame->ip++)
#define READ_CONSTANT() \
	*(Value*)(ARRAY_INDEX(&frame->function->chunk.constants,(READ_BYTE())))

	for(;;){
		disassembleStack(vm);
		uint8_t instruction = READ_BYTE();
		switch (instruction){
			case OP_RETURN:{
					       Value result = pop(vm);
					       vm->frameCount--;
					       if ((vm->frameCount) ==0){
						       pop(vm);
						       return INTERPRET_OK;
					       }
					       vm->stackTop = frame->slots;
					       push(vm,result);
					       frame = getCurrentFrame(vm);
					       break;
				       }
			case OP_PRINT:{
					      Value value = pop(vm);
					      printValue(value);
					      printf("\n");
					      break;
				      }
			case OP_NIL:{
					    push(vm,NIL_VAL);
					    break;
				    }
			case OP_POP:{
					    pop(vm);
					    break;
				    }
			case OP_ADD:{
					    Value b =pop(vm);
					    Value a =pop(vm);
					    push(vm,valueAdd(a,b));
					    break;
				    }
			case OP_DEFINE_GLOBAL:{
						      int offset = READ_BYTE();
						      Value value = *(Value*)ARRAY_INDEX(&frame->function->chunk.constants,offset);
						      tableSet(&vm-> global,AS_STRING(value),pop(vm));
						      break;
					      }
			case OP_CONSTANT:{
						 int offset = READ_BYTE();
						 Value value = *(Value*)(ARRAY_INDEX(&frame->function->chunk.constants,offset));
						 push(vm,value);
						 break;
					 }
			case OP_GET_GLOBAL:{
						   Value key=READ_CONSTANT();
						   Value value;
						   tableGet(&vm-> global,AS_STRING(key),&value);
						   push(vm,value);
						   break;
					   }
			case OP_SET_GLOBAL:{
						   Value key=READ_CONSTANT();
						   Value value = peek(vm,0);
						   tableSet(&vm->global,AS_STRING(key),value);
						   break;
					   }
			case OP_SET_LOCAL:{
						  int offset = READ_BYTE();
						  setStack(vm,offset,peek(vm,0));
						  break;
					  }
			case OP_GET_LOCAL:{
						  int offset = READ_BYTE();
						  push(vm,getStack(vm,offset));
						  break;
					  }
			case OP_CALL:{
					     int argCount = READ_BYTE();
					     printf("argCount:%d\n",argCount);
					     callValue(vm,peek(vm,argCount),argCount);
					     frame = getCurrentFrame(vm);
					     break;
				     }
			default:{
					fprintf(stderr,"vm run error , Unexpect opcode %d\n",instruction);
					exit(64);
				}
		}
	}
}

InterpretResult interpret(char* source){
	VM vm;
	vmInit(&vm);
	ObjFunction* function = compile(source,&vm.strings);
	if (function == NULL){
		return INTERPRET_COMPILE_ERR;
	}
	function->name="main";
	disassembleChunk(&function->chunk,"main");
	push(&vm,OBJ_VAL(function));
	callValue(&vm,OBJ_VAL(function),0);
	return run(&vm);
}
