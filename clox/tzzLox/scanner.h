#ifndef lox_scanner_h
#define lox_scanner_H

#include "token.h"
typedef struct{
	char* source;
	char* start;
	char* current;
}Scanner;

Scanner scanner;

void initScanner(char* source);

Token scanToken();



#endif





