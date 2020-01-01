#ifndef lox_debug_h
#define lox_debug_h

#include "chunk.h"
#include "vm.h"

void disassembleChunk(Chunk* chunk,char* name);
void disassembleStack(VM* vm);
#endif
