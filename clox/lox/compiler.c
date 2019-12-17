#include "compiler.h"
#include "scanner.h"
#include <stdio.h>
#include "chunk.h"
#include <stdlib.h>
#include "string.h"
#include "chunk.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

static void binary();
static void number();
static void unary();
static void group();

typedef enum{
	PREC_NONE,
	PREC_ASSIGNMENT, // = 
	PREC_OR, // or
	PREC_AND,//and
	PREC_EQUALITY, //== !=
	PREC_COMPARSION,// <  <= > >=
	PREC_TERM, // - + 
	PREC_FACTOR, // * /
	PREC_UNARY,// ! -
	PREC_CALL,// . ()
	PREC_PRIMARY,
}Precedence;


typedef void (*ParseFn)();

typedef struct{
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;

}ParseRule;

ParseRule rules[] ={
	{ group, NULL,    PREC_NONE },       // TOKEN_LEFT_PAREN      
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_RIGHT_PAREN     
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_LEFT_BRACE
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_RIGHT_BRACE     
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_COMMA           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_DOT             
	{ unary,    binary,  PREC_TERM },       // TOKEN_MINUS           
	{ NULL,     binary,  PREC_TERM },       // TOKEN_PLUS            
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_SEMICOLON       
	{ NULL,     binary,  PREC_FACTOR },     // TOKEN_SLASH           
	{ NULL,     binary,  PREC_FACTOR },     // TOKEN_STAR            
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_BANG            
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_BANG_EQUAL      
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_EQUAL           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_EQUAL_EQUAL     
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_GREATER         
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_GREATER_EQUAL   
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_LESS            
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_LESS_EQUAL      
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_IDENTIFIER      
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_STRING          
	{ number,   NULL,    PREC_NONE },       // TOKEN_NUMBER          
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_AND             
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_CLASS           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_ELSE            
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_FALSE           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_FOR             
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_FUN             
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_IF              
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_NIL             
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_OR              
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_PRINT           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_RETURN          
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_SUPER           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_THIS            
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_TRUE            
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_VAR             
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_WHILE           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_ERROR           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_EOF
};

static ParseRule* getRule(TokenType type){
	return &rules[type];
}

static void parsePrecedence(Precedence precedence);
static void advance();
static void expression();

typedef struct{
	Token previous;
	Token current;
	bool hadError;
	bool panicMode;
}Parser;

Parser parser;

Chunk* compilingChunk;

static Chunk* currentChunk(){
	return compilingChunk;
}

static void emitByte(uint8_t byte){
	writeChunk(currentChunk(),byte,parser.previous.line);
}

static void errAt(Token *token,char* msg){
	if (parser.panicMode) return;
	parser.panicMode = true;

	fprintf(stderr,"[line %d] Error",token->line);

	if (token->type == TOKEN_EOF){
		fprintf(stderr,"at end");
	}else if (token->type == TOKEN_ERROR){

	}else{
		fprintf(stderr,"at '%.*s'",token->length,token->start);
	}

	fprintf(stderr,": %s\n",msg);
	parser.hadError = true;
}


static void error(char* msg){
	errAt(&parser.previous,msg);
}

static void errAtCurrent(char* msg){
	errAt(&parser.current,msg);
}


void consume(TokenType type,char* msg){
	if (parser.current.type == type){
		advance();
		return;
	}
	errAtCurrent(msg);
}

static void advance(){
	parser.previous = parser.current;
	for(;;){
		parser.current = scanToken();
		if (parser.current.type != TOKEN_ERROR) break;
		errAtCurrent(parser.current.start);
	}

}

static void emitReturn(){
	emitByte(OP_RETURN);
}

static void endCompile(){
	emitReturn();
#ifdef DEBUG_PRINT_CODE
	if (!parser.hadError){
		disassembleChunk(currentChunk(),"code");
	}
#endif
}

static uint8_t makeConstant(Value value){
	int constant = addConstant(currentChunk(),value);
	if (constant > UINT8_MAX){
		error("Too many constants in one chunk");
		return 0;
	}
	return (uint8_t)constant;
}

static void emitConstant(Value value){
	emitByte(OP_CONSTANT);
	emitByte(addConstant(currentChunk(),value));
}

static void number(){
	double  value=strtod(parser.previous.start,NULL);
	emitConstant(value);
}

static void group(){
	expression();
	consume(TOKEN_RIGHT_PAREN,"Expect ')' after expression");
}


static void binary(){
	TokenType operatorType = parser.previous.type;

	//compile the right operand
	ParseRule* rule = getRule(operatorType);
	parsePrecedence((Precedence)(rule->precedence+1));

	switch(operatorType){
		case TOKEN_PLUS:{
					emitByte(OP_ADD);
					break;
				}
		case TOKEN_MINUS:{
					 emitByte(OP_MINUS);
					 break;
				 }
		case TOKEN_SLASH:{
					 emitByte(OP_DIVIDE);
					 break;
				 }
		case TOKEN_STAR:{
					emitByte(OP_MULTIPLY);
					break;
				}
		default:{
				fprintf(stderr,"compile bug!!!! unexpected binary operatorType %d\n",operatorType);
				exit(64);
			}
	}


}

static void unary(){
	TokenType operatorType = parser.previous.type;

	//compile the operand
	parsePrecedence(PREC_UNARY);

	switch(operatorType){
		case TOKEN_MINUS:{
					 emitByte(OP_NEGATE); 
					 break;
				 }
		default: 
				 fprintf(stderr,"compiler bug ,unexpected unary operatorType %d",operatorType);
				 exit(64);
	}
}

static void parsePrecedence(Precedence precedence){
	advance();
	ParseFn prefixRule = getRule(parser.previous.type)->prefix;
	if (prefixRule ==NULL){
		error("Expect expression");
		return;
	} 
	prefixRule();

	while(precedence <= getRule(parser.current.type)->precedence){
		advance();
		ParseFn infixRule = getRule(parser.previous.type)->infix;
		infixRule();
	}
}

static void expression(){
	parsePrecedence(PREC_ASSIGNMENT);
}

bool compile(char* source,Chunk* chunk){
	initScanner(source);
	compilingChunk = chunk;
	parser.hadError  =false;
	parser.panicMode = false;
	advance();
	expression();

	consume(TOKEN_EOF,"Expect enf of expression.");
	endCompile();


	//int line = -1;
	//for (;;){
	//	Token token = scanToken();
	//	if (token.line != line){
	//		printf("%4d ",token.line);
	//		line = token.line;
	//	}else{
	//		printf("  | ");
	//	}

	//	printf("%2d '%.*s'\n",token.type,token.length,token.start);

	//	if (token.type == TOKEN_EOF ){
	//		break;
	//	}
	//}
	return !parser.hadError;
}


