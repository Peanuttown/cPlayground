#ifndef lox_parser_h
#define lox_parser_h
#include "array.h"


typedef struct{
	Array* tokens; 
	int current;
	Array code;
	Array constant;//elemType Value
}Parser;

Parser parser;

void initParser(Array* tokens);
void parse();

#endif
