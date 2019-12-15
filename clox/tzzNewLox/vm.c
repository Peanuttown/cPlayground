#include "vm.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include <errno.h>
#include "memory.h"
#include "scanner.h"
#include "parser.h"
#include "array.h"
#include "opcode.h"
#include "value.h"
#include "table.h"
#include "stringObject.h"


#define getCodeByIndex(index) \
	(*(uint8_t*)(getArray(&vm.code,index)))

#define getConstantByIndex(index) \
	(Value*)(getArray(&vm.constants,index))


extern VM vm;

void initVM(Array code,Array constants){
	initStack(&vm.stack);
	vm.code = code;
	vm.constants = constants;
	vm.ip =0;
	initTable(&vm.strings);
}

static bool isEnd(){
	return vm.ip >= vm.code.count;
}

uint8_t advance(){
	uint8_t code = getCodeByIndex(vm.ip);
	vm.ip++;
	return code;
}


InterpretResult interpret(){
	printf("start interpret\n");
	for(;!isEnd();){
		uint8_t code = advance();
		switch(code){
			case OP_RETURN:{
					       return INTERPRETER_OK;
				       }
			case OP_CONSTANT:{
						 //get constant by next op_code;
						 int constantPos = advance();
						 push(&vm.stack,getConstantByIndex(constantPos));
						 break;
					 }
			case OP_PRINT:{
					      Value* value = pop(&vm.stack);
					      printValue(value);
					      break;
				      }
			case OP_GET_GLOBAL:{
						   uint8_t pos = advance();
						   Value* variable = getConstantByIndex(pos);
						   ObjString* key = (ObjString*)(variable->obj);
						   Value toPush;
						   TableGet(&vm.global,key,&toPush);
						   push(&vm.stack,&toPush);
						   break;
					   }
			case OP_DEFINE_GLOBAL:{
						      uint8_t pos = advance();
						      Value* variable = getConstantByIndex(pos);
						      ObjString* key = (ObjString*)(variable->obj);
						      TableSet(&vm.global,key,pop(&vm.stack));
						      break;
					      }
			case OP_SET_GLOBAL:{
						   int pos = advance();
						   Value* variable = getConstantByIndex(pos);
						   ObjString* key = (ObjString*)(variable->obj);
						   Value* value =pop(&vm.stack);
						   TableSet(&vm.global,key,value);
						   push(&vm.stack,value);
						   break;
					   }
			case OP_POP:{
					    pop(&vm.stack);
					    break;
				    }

			case OP_SET_LOCAL:{
						  int pos =advance();
						  vm.stack.Values[pos] = vm.stack.Values[vm.stack.top-1];
						  break;
					  }
			case OP_GET_LOCAL:{
						  int pos =advance();
						  push(&vm.stack,&vm.stack.Values[pos]);
						  break;
					  }
			default:{
					printf("undefined opcode %d\n",code);
					return INTERPRETER_RUNTIME_ERR;
				}

		}
	}
	return INTERPRETER_OK;
}

InterpretResult run(char* source){
	initScanner(source);
	if (!Scan()){
		return INTERPRETER_COMPILED_ERR;
	}
	initParser(&scanner.tokens);
	parse();
	initVM(parser.code,parser.constant);
	disassembleChunk(&vm);
	return interpret();
}

InterpretResult runFile(char* filename){
	FILE* file = fopen(filename,"r");
	if (file == NULL){
		fprintf(stderr,"open fail error :%s\n",strerror(errno));
		return INTERPRETER_COMPILED_ERR;
	}
	//read file
	//get file length
	fseek(file,0,SEEK_END);
	int fileLength = ftell(file);
	rewind(file);
	char* source= allocMemory(fileLength+1);
	source[fileLength] = '\0';
	int bytesRead = fread(source,sizeof(char),fileLength,file);
	if (bytesRead != fileLength){
		fprintf(stderr,"read file error,read byte :%d, expect read byte:%d\n",bytesRead,fileLength);
		return INTERPRETER_COMPILED_ERR;
	}
	return run(source);
}

#undef getCodeByIndex

