#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum{
	OP_RETURN,
	OP_CONSTANT,
}OpCode;

typedef struct{
	int count;
	int capacity;
	uint8_t* code;//save bytecode
	int* line;// one code to one line
	ValueArray constants;
}Chunk;

void initChunk(Chunk* chunk);
void writeByte(Chunk* chunk,uint8_t bytecode,int line);
void freeChunk(Chunk* chunk);

#endif
