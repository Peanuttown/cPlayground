#ifndef lox_debug_h
#define lox_debug_h
#include "chunk.h"

void disassembleChunk(Chunk* chunk,char* chunkName);
int disassembleInstruction(Chunk* chunk,int offset);

#endif
