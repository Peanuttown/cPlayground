#ifndef lox_scanner_h
#define lox_scanner_h

#include "token.h"
typedef struct{
	char* source;
	char* current;
	char* start;
	int line;
}Scanner;

void scannerInit(Scanner* scanner,char* source);

Token scanToken(Scanner* scanner);


#endif
