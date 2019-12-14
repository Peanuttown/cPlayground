#ifndef lox_debug_h
#define lox_debug_h
#include "vm.h"
#include "array.h"

int disassembleOpCode(VM* vm,int offset,uint8_t opcode);

void disassembleChunk(VM* vm);

#endif
