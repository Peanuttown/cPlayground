#ifndef lox_chunk_h
#define lox_chunk_h

#include "common.h"
#include "value.h"
typedef enum{
	OP_PRINT,
	OP_RETURN,
	OP_CONSTANT,
	OP_NIL,
	OP_TRUE,
	OP_FALSE,
	OP_EQUAL,
	OP_GREATER,
	OP_LESS,
	OP_NEGATE,
	OP_ADD,
	OP_MINUS,
	OP_DIVIDE,
	OP_NOT,
	OP_MULTIPLY,
	OP_POP,
	OP_DEFINE_GLOBAL,
	OP_GET_GLOBAL,
	OP_SET_GLOBAL,
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

