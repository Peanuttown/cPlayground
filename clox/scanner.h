#ifndef clox_scanner_h
#define clox_scanner_h

#include "token.h"

typedef struct{
	char* start;
	char* current;
	char* source;
	int line ;
}Scanner;

void initScanner(Scanner* scanner,char* source);

Token scanToken(Scanner* scanner);



#endif
