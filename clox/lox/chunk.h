#ifndef lox_chunk_h
#define lox_chunk_h

#include "common.h"
#include "value.h"
typedef enum{
	OP_RETURN,
	OP_CONSTANT,
	OP_NEGATE,
	OP_ADD,
	OP_MINUS,
	OP_DIVIDE,
	OP_MULTIPLY,
}OpCode;

typedef struct{
	int count;
	int cap;
	uint8_t* code;
	ValueArray constants;
	int* lines;
}Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk,uint8_t byte,int line);
void freeChunk(Chunk* chunk);
int addConstant(Chunk* chunk,Value value);



#endif

