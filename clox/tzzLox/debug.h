#ifndef lox_debug_h
#define lox_debug_h

#include "chunk.h"

void disassembleChunk(Chunk* chunk);

int disassembaleInstruction(Chunk* chunk,int offset,uint8_t instruction);

#endif
