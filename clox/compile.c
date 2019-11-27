#include "compile.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

static void advance(Parser* parser){
	parser->previous = parser->current;
	for(;;){
		parser->current = scanToken(parser->scanner);
		if (parser->current.type != TOKEN_ERROR) break;
		fprintf(stderr,"get one error token\n");
		exit(64);
	}
}


void consume(Parser* parser,TokenType type,char* errMsg){
	if (parser->current.type != type){
		fprintf(stderr,errMsg);
		exit(64);
	} 
	advance(parser);
}

static void expression(Parser* parser){
	//todo
}

void grouping(Parser* parser){
	expression(parser);
	consume(parser,TOKEN_RIGHT_PAREN,"expect )\n");
}

void number(Parser* parser){
	//todo
}

void binary(Parser* parser){

}

void unary(Parser* parser){
	TokenType operator = parser->previous.type;
	parsePrecedence(PREC_UNARY);

	//todo emit byte;
}

ParseRule rules[] = {
	{ grouping, NULL,    PREC_NONE },       // TOKEN_LEFT_PAREN      
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

static ParseRule* getRule(TokenType type) {
	return &rules[type];                     
}


static void parsePrecedence(Parser* parser,Parcedence precedence){
	advance(parser);
	ParseFn prefixRule = getRule(parser->previous.type)->prefix;
	if ( prefixRule == NULL){
		fprintf(stderr,"expect expression\n");
		exit(64);
	}
	prefixRule(parser);
	while (precedence <= getRule(parser.current.type)->precedence) {
		advance();                                                    
		ParseFn infixRule = getRule(parser.previous.type)->infix;     
		infixRule();
	}
}

void initParser(Parser* parser,Scanner* scanner){
	parser->scanner = scanner;
}

static void express(Parser* parser){

}

void compile(char* source){
	Scanner scanner;
	initScanner(&scanner,source);
	Parser parser;
	initParser(&parser,&scanner);
	advance(&parser);
}


