#ifndef lox_parser_h
#define lox_parser_h
#include "token.h"


typedef struct{
	Token previous;
	Token current;
}Parser;

Parser parser;

#endif
