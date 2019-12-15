#ifndef lox_opcode_h
#define lox_opcode_h
#include "common.h"

#define GET_OPCODE(code)\
	(uint8_t)(code)

typedef enum{
	OP_RETURN,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_NEGATE,
	OP_CONSTANT,
	OP_JUMP_IF_FALSE,
	OP_JUMP,
	OP_POP,
	OP_PRINT,
	OP_EQUAL_EQUAL,
	OP_BANG_EQUAL,
	OP_LESS,
	OP_LESS_EQUAL,
	OP_GREATER,
	OP_GREATER_EQUAL,
	OP_GET_GLOBAL,
	OP_NIL,
	OP_DEFINE_GLOBAL,
	OP_SET_GLOBAL,
	OP_SET_LOCAL,
	OP_GET_LOCAL,
}OpCode;

#endif


