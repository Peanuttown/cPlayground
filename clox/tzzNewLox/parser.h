#ifndef lox_parser_h
#define lox_parser_h
#include "array.h"
#include "token.h"


#define LOCAL_MAX 256

typedef struct{
	Array* tokens; 
	int current;
	Array code;
	Array constant;//elemType Value
}Parser;

Parser parser;

typedef struct{
	Token name;
	int depth;
}Local;

typedef struct{
	Local locals[LOCAL_MAX];
	int localCount;
	int scopeDepth;

}Compiler;

Compiler current;

void initParser(Array* tokens);
void parse();

#endif
