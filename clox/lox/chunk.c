#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk* chunk){
	chunk->cap = 0;
	chunk->count = 0;
	chunk->code =NULL;
	chunk->lines = NULL;
	initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk,uint8_t byte,int line){
	if (chunk->count + 1 > chunk->cap){
		int capacity  = GROW_CAPACITY(chunk->cap);
		chunk->code = GROW_ARRAY(chunk->code,uint8_t,chunk->cap,capacity);
		chunk->lines = GROW_ARRAY(chunk->lines,int,chunk->cap,capacity);
		chunk->cap = capacity;
	}
	chunk->code[chunk->count] = byte;
	chunk->lines[chunk->count] = line;
	chunk->count++;
}

void freeChunk(Chunk* chunk){
	FREE_ARRAY(uint8_t,chunk->code,chunk->cap);
	FREE_ARRAY(int,chunk->lines,chunk->cap);
	freeValueArray(&chunk->constants);
	initChunk(chunk);
}

int addConstant(Chunk* chunk,Value value){
	writeValueArray(&chunk->constants,value);
	return chunk->constants.count - 1;
}
