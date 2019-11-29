#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include "value.h"


void disassembleChunk(Chunk* chunk){
	printf("%s|%10s|%15s|%15s\n","offset","opcode","constantPos","constantValue");
	for(int offset=0;offset < chunk->count;){
		offset = disassembaleInstruction(chunk,offset,chunk->code[offset]);
	}
}

static int simpleInstruction(char* opcodeName,int offset){
	printf("%6d|%10s\n",offset,opcodeName);
	return offset + 1;
}

static int constantInstruction(Chunk* chunk,char* name,int offset){
	//get constant
	int valuePos = chunk->code[offset+1];
	Value value = chunk->constants.values[valuePos];
	printf("%6d|%10s|%15d|%15g\n",offset,name,valuePos,AS_NUMBER(value));;
	return offset+2;
}

int disassembaleInstruction(Chunk* chunk,int offset,uint8_t instruction){
	switch(instruction){
		case OP_RETURN:return simpleInstruction("OP_RETURN",offset);
		case OP_CONSTANT:return constantInstruction(chunk,"OP_CONSTANT",offset);
		case OP_ADD:return simpleInstruction("OP_ADD",offset);
		case OP_SUB:return simpleInstruction("OP_SUB",offset);
		case OP_MUL:return simpleInstruction("OP_MUL",offset);
		case OP_DIV:return simpleInstruction("OP_DIV",offset);
		case OP_NEGATE:return simpleInstruction("OP_NEGATE",offset);
		case OP_NIL:return simpleInstruction("OP_NIL",offset);
		case OP_FALSE:return simpleInstruction("OP_FALSE",offset);
		case OP_TRUE:return simpleInstruction("OP_TRUE",offset);
		default:{
				fprintf(stderr,"undefined op_code %d\n",instruction);
				exit(1);
			}
	}
}


