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
	initTable(&vm->glboal);
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

static bool call(VM* vm,ObjFunction* function,int argc){
	if (argc!=function->arity){
		fprintf(stderr,"function arity not match\n");
		return false;
	}
	if (vm->frameCount == FRAME_MAX){
		fprintf(stderr,"Stack overflow\n");
		return false;
	}
	CallFrame* frame = &vm->callFrames[vm->frameCount++];
	frame->function = function;
	frame->ip = (uint8_t*)function->chunk.codes.elems;
	frame->slots = vm->stackTop -argc-1;
	return true;
}

static bool callValue(VM* vm,Value callee,int argc){
	if (IS_OBJ(callee)){
		switch(OBJ_TYPE(callee)){
			case OBJ_FUNCTION:{
						  return call(vm,AS_FUNCTION(callee),argc);
					  }
			default:{
					fprintf(stderr,"Unexpected callable object type %d\n",OBJ_TYPE(callee));
					return false;
				}
		}

	}else{
		fprintf(stderr,"callValue:callee is not object");
		return false;
	}
}


static CallFrame* getCurrentFrame(VM* vm){
	if (vm->frameCount==0){
		fprintf(stderr,"no current call frame\n");
		exit(64);
	}
	return &vm->callFrames[vm->frameCount-1];
}

InterpretResult run(VM* vm){
	CallFrame* frame = getCurrentFrame(vm);

#define READ_BYTE() (*frame->ip++)
	for(;;){
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
						      tableSet(&vm->glboal,AS_STRING(value),pop(vm));
						      break;
					      }
			case OP_CONSTANT:{
						 int offset = READ_BYTE();
						 Value value = *(Value*)(ARRAY_INDEX(&frame->function->chunk.constants,offset));
						 push(vm,value);
						 break;
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

	disassembleChunk(&function->chunk,"main");
	push(&vm,OBJ_VAL(function));
	callValue(&vm,OBJ_VAL(function),0);
	return run(&vm);
}
