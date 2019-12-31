#ifndef lox_chunk_h
#define lox_chunk_h

#include "common.h"
#include "array.h"
#include "value.h"



typedef enum{
	OP_RETURN,
	OP_CONSTANT,
	OP_ADD,
	OP_POP,
	OP_NIL,
	OP_PRINT,
}OpCode;

typedef struct{
	Array codes;
	Array constants;
	Array lines;
}Chunk;

void chunkInit(Chunk* chunk);
void chunkFree(Chunk* chunk);
int addConstant(Chunk* chunk,Value);

#endif
