#ifndef clox_compile_h
#define clox_compile_h

#include "chunk.h"
#include "scanner.h"
void compile(char* source);

typedef struct{
	Token previous;
	Token current;
	Scanner* scanner;
}Parser;


typedef enum {                  
  PREC_NONE,                    
  PREC_ASSIGNMENT,  // =        
  PREC_OR,          // or       
  PREC_AND,         // and      
  PREC_EQUALITY,    // == !=    
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -      
  PREC_FACTOR,      // * /      
  PREC_UNARY,       // ! -      
  PREC_CALL,        // . () []  
  PREC_PRIMARY                  
} Precedence;  

typedef void (*ParseFn)(Parser*);

typedef struct {        
  ParseFn prefix;       
  ParseFn infix;        
  Precedence precedence;
} ParseRule;


void initParser(Parser* parser,Scanner* scanner);

#endif
