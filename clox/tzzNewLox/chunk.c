#include "chunk.h"
#include "array.h"
#include "common.h"
#include "value.h"

void chunkInit(Chunk* chunk){
	ARRAY_INIT(&chunk->codes,uint8_t);
	ARRAY_INIT(&chunk->constants,Value);
	ARRAY_INIT(&chunk->lines,int);
}

static void freeConstants(Chunk* chunk){
	//todo
}

void chunkFree(Chunk* chunk){
	arrayFree(&chunk->codes);
	freeConstants(chunk);
	arrayFree(&chunk->lines);
}

int addConstant(Chunk* chunk,Value value){
	return arrayWrite(&chunk->constants,&value);
}
