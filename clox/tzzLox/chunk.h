#ifndef lox_chunk_h
#define lox_chunk_h

#include "common.h"
#include "value.h"

typedef enum{
	OP_RETURN,
	OP_CONSTANT,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_NEGATE,
	OP_NIL,
	OP_FALSE,
	OP_TRUE,
}OpCode;

typedef struct{
	int count;
	int capacity;
	uint8_t* code;
	ValueArray constants;
}Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk,uint8_t bytecode);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk,Value value);


#endif
