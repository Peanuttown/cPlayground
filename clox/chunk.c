#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk* chunk){
	chunk->count =0;
	chunk->capacity = 0;
	chunk->code = NULL;
	chunk->line = NULL;
	initValueArray(&chunk->constants);
}

void writeByte(Chunk* chunk,uint8_t bytecode,int line){
	if (chunk->capacity < chunk->count + 1){//need expand memory?
		chunk->capacity = GROW_CAPACITY(chunk->capacity);
		chunk->code = GROW_ARRAY(chunk->code,uint8_t,chunk->capacity);
		chunk->line = GROW_ARRAY(chunk->line,int,chunk->capacity);
	}
	chunk->code[chunk->count] = bytecode;
	chunk->line[chunk->count] = line;
	chunk->count++;
}

int addConstant(Chunk* chunk,Value value){
	writeValueArray(&chunk->constants,value);
	return chunk->constants.count-1;
}

void freeChunk(Chunk* chunk){
	freeValueArray(&chunk->constants);
	FREE_ARRAY(chunk->code,uint8_t,chunk->capacity);
	FREE_ARRAY(chunk->line,int,chunk->capacity);
	initChunk(chunk);
}
