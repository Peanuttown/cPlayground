#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk){
	chunk->count = 0;
	chunk->capacity =0;
	chunk->code =NULL;
	initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk,uint8_t bytecode){
	if (chunk->capacity < chunk->count +1){
		chunk->capacity = (chunk->capacity<8?8:chunk->capacity*2);
		chunk->code = realloc(chunk->code,sizeof(uint8_t)*chunk->capacity);
	}
	chunk->code[chunk->count] = bytecode;
	chunk->count ++;

}

void freeChunk(Chunk* chunk){
	freeValueArray(&chunk->constants);
	free(chunk->code);
	chunk->count =0;
	chunk->capacity =0;
	chunk->code =NULL;
}

int addConstant(Chunk* chunk,Value value){
	return writeValueArray(&chunk->constants,value);
}

