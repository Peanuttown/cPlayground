#include "vm.h"
#include "chunk.h"
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "compiler.h"
#include <stdarg.h>
#include "object.h"
#include "memory.h"
#include "string.h"
#include "table.h"

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
	vm.objects = NULL;
	initTable(&vm.strings);
	initTable(&vm.globals);
}


static void freeObject(Obj* obj){
	switch(obj->type){
		case OBJ_STRING:{
					ObjString* string = (ObjString*)obj;
					FREE_ARRAY(char,string->chars,string->length+1);
					FREE(ObjString,string);
					break;
				}
		default:{
				fprintf(stderr,"freeObject error : unexpect obj type %d\n",obj->type);
				exit(64);
			}

	}

}
static void freeObjects(){
	Obj* obj = vm.objects;
	for (;obj != NULL;){
		Obj* next = obj->next;
		freeObject(obj);
		obj = next;
	}

}

void freeVM(){
	freeObjects();
	freeTable(&vm.strings);
	freeTable(&vm.globals);
}


static Value peek(int distance){
	return vm.stackTop[-1-distance];
}

static void runtimeError(char* msg,...){
	va_list args;
	va_start(args,msg);
	vfprintf(stderr,msg,args);
	va_end(args);
	fputs("\n",stderr);

	//get instruction
	uint8_t  instruction = vm.ip - vm.chunk->code;
	//get line
	int line = vm.chunk->lines[instruction];
	fprintf(stderr,"[line %d] in script",line);
	resetStack();
}

static bool isFalsey(Value value){
	return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));

}


static void concatenate(){
	ObjString* b = AS_STRING(pop());
	ObjString* a = AS_STRING(pop());

	int length = a->length + b->length;
	char* chars = ALLOCATE(char,length+1);
	memcpy(chars,a->chars,a->length);
	memcpy(chars+a->length,b->chars,b->length);

	chars[length] = '\0';
	ObjString* result = takeString(chars,length+1);
	push(OBJ_VAL(result));

}

static InterpretResult run(){
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType,op) \
	do{ \
		if (IS_STRING(peek(0))&& IS_STRING(peek(1))){\
			concatenate();\
		}else if(!IS_NUMBER((peek(0))) || !IS_NUMBER(peek(1))){\
			runtimeError("Operands must be number"); \
			return INTERPRET_RUNTIME_ERR; \
		}\
		double b = AS_NUMBER(pop()); \
		double a = AS_NUMBER(pop()); \
		push(valueType(a op b)); \
	}while(false) 

#define READ_STRING() AS_STRING(READ_CONSTANT())

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
					       return INTERPRET_OK;
				       }
			case OP_CONSTANT:{
						 Value value = READ_CONSTANT();
						 push(value);
						 break;
					 }
			case OP_ADD:{
					    BINARY_OP(NUMBER_VAL,+);
					    break;
				    }
			case OP_MINUS:{
					      BINARY_OP(NUMBER_VAL,-);
					      break;
				      }
			case OP_DIVIDE:{
					       BINARY_OP(NUMBER_VAL,/);
					       break;
				       }
			case OP_MULTIPLY:{
						 BINARY_OP(NUMBER_VAL,*);
						 break;
					 }
			case OP_NEGATE:{
					       if (!(IS_NUMBER(peek(0)))){
						       runtimeError("negate operand must be number");
						       return INTERPRET_RUNTIME_ERR;
					       }
					       push(NUMBER_VAL(-AS_NUMBER(pop())));
				       }
			case OP_FALSE:{
					      push(BOOL_VAL(false));
					      break;
				      }
			case OP_TRUE:{
					     push(BOOL_VAL(true));
					     break;
				     }
			case OP_NIL:{
					    push(NIL_VAL);
					    break;
				    }
			case OP_NOT:{
					    push(BOOL_VAL(isFalsey(pop())));
					    break;
				    }
			case OP_EQUAL:{
					      Value b = pop();
					      Value a = pop();
					      push(BOOL_VAL(valuesEqual(a,b)));
				      }
			case OP_GREATER:{
						BINARY_OP(BOOL_VAL,>);
						break;
					}
			case OP_LESS:{
					     BINARY_OP(BOOL_VAL,<);
					     break;
				     }
			case OP_PRINT:{
					      printValue(pop());
					      printf("\n");
					      break;
				      }
			case OP_POP:{
					    pop();
					    break;
				    }
			case OP_DEFINE_GLOBAL:{
						      ObjString* string = READ_STRING();
						      tableSet(&vm.globals,string,peek(0));
						      pop();
						      break;
					      }
			case OP_GET_GLOBAL:{
						   ObjString* name = READ_STRING();
						   Value value;
						   if(!tableGet(&vm.globals,name,&value)){
							   runtimeError("Undefined variable '%s'",name->chars);
							   return INTERPRET_RUNTIME_ERR;
						   }
						   push(value);
						   break;
					   }
			case OP_SET_GLOBAL:{
						   ObjString* name= READ_STRING();
						   Value value = pop();
						   tableSet(&vm.globals,name,value);
						   break;
					   }
			default:{
					fprintf(stderr,"interpret error ,unexpected instruction :%d",instruction);
					exit(64);
				}
		}
	}
#undef READ_BYTE
#undef READ_CONSTANT
#undef READ_STRING
	return 0;
}

InterpretResult interpret(char* source){
	Chunk chunk;
	initChunk(&chunk);
	if (!compile(source,&chunk)){
		freeChunk(&chunk);
		return INTERPRET_COMPILE_ERR;
	}

	vm.chunk = &chunk;
	vm.ip = vm.chunk->code;
	InterpretResult result = run();
	freeChunk(&chunk);
	return result;
}
