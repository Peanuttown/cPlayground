#include "debug.h"
#include <stdio.h>
#include "value.h"
#include <stdlib.h>

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

static int jumpInstruction(char* name,int sign,Chunk* chunk,int offset){
	uint16_t  jump = (uint16_t)chunk->code[offset+1]<<8;
	jump |= chunk->code[offset+2];
	printf("%-16s %4d->%d\n",name,offset,offset+3+sign*jump);
	return offset+3;

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
		case OP_ADD:{
					 return printSimpleInstruction("OP_ADD",offset);
				 }
		case OP_MINUS:{
					 return printSimpleInstruction("OP_MINUS",offset);
				 }
		case OP_DIVIDE:{
					 return printSimpleInstruction("OP_DIVIDE",offset);
				 }
		case OP_MULTIPLY:{
					 return printSimpleInstruction("OP_MULTIPLY",offset);
				 }
		case OP_FALSE:{
					 return printSimpleInstruction("OP_FALSE",offset);
			      }
		case OP_TRUE:{
					 return printSimpleInstruction("OP_TRUE",offset);
			     }
		case OP_NIL:{
					 return printSimpleInstruction("OP_NIL",offset);
			    }
		case OP_NOT:{
					 return printSimpleInstruction("OP_NOT",offset);
			    }
		case OP_EQUAL:{
					 return printSimpleInstruction("OP_EQUAL",offset);
			    }
		case OP_GREATER:{
					 return printSimpleInstruction("OP_GREATER",offset);
			    }
		case OP_LESS:{
					 return printSimpleInstruction("OP_LESS",offset);
			    }
		case OP_PRINT:{
				      return printSimpleInstruction("OP_PRINT",offset);
			      }
		case OP_POP:{
				      return printSimpleInstruction("OP_POP",offset);
			      }
		case OP_DEFINE_GLOBAL:{
					 return constantInstruction("OP_DEFINE_GLOBAL",chunk,offset);
			      }
		case OP_GET_GLOBAL:{
					 return constantInstruction("OP_GET_GLOBAL",chunk,offset);
				   }
		case OP_SET_GLOBAL:{
					 return constantInstruction("OP_SET_GLOBAL",chunk,offset);
				   }
		case OP_SET_LOCAL:{
					 return constantInstruction("OP_SET_LOCAL",chunk,offset);
				   }
		case OP_GET_LOCAL:{
					 return constantInstruction("OP_GET_LOCAL",chunk,offset);
				   }
		case OP_JUMP_IF_FALSE:{
					      return jumpInstruction("OP_JUMP_IF_FALSE",1,chunk,offset);
				      }
		case OP_JUMP:{
					      return jumpInstruction("OP_JUMP",1,chunk,offset);
				      }
		case OP_LOOP:{
				     return jumpInstruction("OP_LOOP",-1,chunk,offset);
			     }
		default:{
				printf("debug error : unknown instruction type %d\n",code);
				exit(64);
			}
	}
}
