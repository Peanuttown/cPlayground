#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include "chunk.h"
#include "array.h"

static int getChunkLine(Chunk* chunk,int offset){
	return (*(int*)ARRAY_INDEX(&chunk->lines,offset));
}

static int printSimpileInstruction(int offset,char* name){
	printf("%s\n",name);
	return offset+1;
}

static int constantInstruction(Chunk* chunk,int offset,char* name){
	uint8_t constant = *(uint8_t*)ARRAY_INDEX(&chunk->codes,offset+1);
	printf("%-16s  %4d ",name,constant);
	printValue(*(Value*)(ARRAY_INDEX(&chunk->constants,constant)));
	printf("\n");
	return offset+2;
}

static int disassembleInstruction(Chunk* chunk,int offset){
	uint8_t* instruction= ARRAY_INDEX(&chunk->codes,offset);
	int line =getChunkLine(chunk,offset);

	if (offset!=0&&line == getChunkLine(chunk,offset-1)){
		printf("%4s  offset:%d  ","|",offset);
	}else{
		printf("%4d  offset:%d  ",line,offset);
	}

	switch (*instruction){
		case OP_RETURN:{
				       return printSimpileInstruction(offset,"op_return");
			       }
		case OP_PRINT:{
				      return printSimpileInstruction(offset,"op_print");
			      }
		case OP_CONSTANT:{
					 return constantInstruction(chunk,offset,"op_constant");
				 }
		case OP_ADD:{
				    return printSimpileInstruction(offset,"op_add");
			    }
		case OP_NIL:{
				    return printSimpileInstruction(offset,"op_nil");
			    }
		case OP_POP:{

				    return printSimpileInstruction(offset,"op_pop");
			    }
		case OP_DEFINE_GLOBAL:{
					      return constantInstruction(chunk,offset,"op_define_global");
				      }
		default:{
				fprintf(stderr,"disassembleInstruction error:Unexpect opCode type %d",*instruction);
				exit(64);
			}
	}
	return 0;
}

void disassembleChunk(Chunk* chunk,char* name){
	printf("chunk :%s\n",name);
	for(int count = 0;count < arrayGetCount(&chunk->codes);){
		count =disassembleInstruction(chunk,count);
	}

}
