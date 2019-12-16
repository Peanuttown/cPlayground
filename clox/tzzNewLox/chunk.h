#ifndef lox_chunk_h
#define lox_chunk_h

#include "array.h"

typedef struct{
	Array code;
}Chunk;

void initChunk(Chunk* chunk);

#endif
