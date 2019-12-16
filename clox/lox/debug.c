#include "debug.h"
#include <stdio.h>
#include "value.h"

static int printSimpleInstruction(char* name,int offset){
	printf("%s\n",name);
	return offset+1;
}

void disassembleChunk(Chunk* chunk,char* chunkName){
	printf("== %s ==\n",chunkName);
	for(int offset =0;offset < chunk->count;){
		offset = disassembleInstruction(chunk,offset);
	}
}

static int constantInstruction(char* name,Chunk* chunk,int offset){
	uint8_t constant = chunk->code[offset+1];
	printf("%-16s  %4d ",name,constant);
	printValue(chunk->constants.values[constant]);
	printf("\n");
	return offset+2;
}

int disassembleInstruction(Chunk* chunk,int offset){
	printf("%04d    ",offset);
	
	if (offset>0 && (chunk->lines[offset] == chunk->lines[offset-1])){
		printf("   | ");
	}else{
		printf("%4d ",chunk->lines[offset]);
	}
	uint8_t code = chunk->code[offset];
	switch(code){
		case OP_RETURN:{
				       return printSimpleInstruction("OP_RETURN",offset);
			       }

		case OP_CONSTANT:{
					 return constantInstruction("OP_CONSTANT",chunk,offset);
				 }
		default:{
				printf("unknown instruction type %d\n",code);
				return offset+1;
			}
	}
}
