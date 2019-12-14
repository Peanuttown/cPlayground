#ifndef lox_scanner_h
#define lox_scanner_h


#include "common.h"
#include "token.h"
#include "array.h"

typedef struct{
	char* source; //source code
	int current ;//current token to scan
	int start;
	Array tokens;//to save the result of scan;
	int line;
}Scanner;

Scanner scanner;//define a global scanner,fang bian operate

void initScanner(char* source);


bool Scan();

void addToken();

void printScanner();

#endif


