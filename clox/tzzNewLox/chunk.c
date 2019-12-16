#include "chunk.h"
#include "array.h"

void initChunk(Chunk* chunk){
	initArray(&chunk->code,sizeof(uint8_t));
}


