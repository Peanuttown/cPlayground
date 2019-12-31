#include "compile.h"
#include <stdlib.h>
#include "object.h"
#include "scanner.h"
#include "token.h"
#include <stdio.h>
#include "value.h"
#include <string.h>
#include "chunk.h"
#include "table.h"

typedef enum{
	PREC_NONE,
	PREC_ASSIGNMENT,
	PREC_EQUALITY,
	PREC_COMPARSION,
	PREC_TERM, // - + 
	PREC_FACTOR, // * /
	PREC_UNARY,// ! -
	PREC_CALL,// . ()
	PREC_PRIMARY,
}Precedence;

typedef struct{
	ObjFunction* function;
	Scanner* scanner;
	Token currentToken;
	Token previousToken;
	Table strings;
}Compiler;

typedef void(*ParseFn)(Compiler* compiler);

typedef struct{
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;
}ParseRule;



static void compilerInit(Compiler* compiler,Scanner* scanner){
	compiler->scanner = scanner;
	compiler->function =newFunction();
}

static Chunk* getChunk(Compiler* compiler){
	return &compiler->function->chunk;
}

static bool isEnd(Compiler* compiler){
	return compiler->currentToken.type == TOKEN_EOF;
}

static void advance(Compiler* compiler){
	compiler->previousToken = compiler->currentToken;
	compiler->currentToken = scanToken(compiler->scanner);
}

static bool match(Compiler* compiler,TokenType type){
	if (compiler->currentToken.type == type){
		advance(compiler);
		return true;
	}
	return false;
}

void emitByte(Compiler* compiler,uint8_t code){
	arrayWrite(&getChunk(compiler)->codes,&code);
	arrayWrite(&getChunk(compiler)->lines,&compiler->previousToken.line);;
}

void emitConstant(Compiler* compiler,Value value){
	Chunk* chunk = &compiler->function->chunk;
	int offset = addConstant(chunk,value);
	emitByte(compiler,OP_CONSTANT);
	emitByte(compiler,offset);
}

static ParseRule getRule(TokenType type);
static void parsePrecedence(Compiler* compiler,Precedence precedence);

void number(Compiler *compiler){
	Token numberToken = compiler->previousToken;
	double value = strtod(numberToken.name,NULL);
	emitConstant(compiler,NUMBER_VAL(value));
}

void binary(Compiler* compiler){
	Token operator = compiler->previousToken;

	//parse right operand
	ParseRule rule = getRule(operator.type);
	parsePrecedence(compiler,rule.precedence+1);


	Chunk* chunk = &compiler->function->chunk;
	switch(operator.type){
		case TOKEN_PLUS:{
					emitByte(compiler,OP_ADD);
					break;
				}
		default:{
				fprintf(stderr,"binary error,undefine operaotr %d\n",operator.type);
				exit(64);
			}
	}
}


static void string(Compiler* compiler){
	Token stringToken = compiler->previousToken;
	copyString(compiler);

}

const ParseRule rules[]={
	{NULL,NULL,PREC_NONE},//TOKEN_EOF,
	{NULL,NULL,PREC_NONE},//TOKEN_IDENTIFIER,
	{NULL,NULL,PREC_NONE},//TOKEN_THIS,
	{NULL,NULL,PREC_NONE},//TOKEN_FOR,
	{NULL,NULL,PREC_NONE},//TOKEN_FUNC,
	{NULL,binary,PREC_TERM},//TOKEN_PLUS,
	{NULL,NULL,PREC_NONE},//TOKEN_MINUS,
	{NULL,NULL,PREC_NONE},//TOKEN_STAR,
	{NULL,NULL,PREC_NONE},//TOKEN_SLASH,
	{NULL,NULL,PREC_NONE},//TOKEN_EQUAL,
	{NULL,NULL,PREC_NONE},//TOKEN_EQUAL_EQUAL,
	{NULL,NULL,PREC_NONE},//TOKEN_BANG,
	{NULL,NULL,PREC_NONE},//TOKEN_BANG_EQUAL,
	{NULL,NULL,PREC_NONE},//TOKEN_LESS,
	{NULL,NULL,PREC_NONE},//TOKEN_LESS_EQUAL,
	{NULL,NULL,PREC_NONE},//TOKEN_GREATER,
	{NULL,NULL,PREC_NONE},//TOKEN_GREATER_EQUAL,
	{number,NULL,PREC_PRIMARY},//TOKEN_NUMBER,

	{NULL,NULL,PREC_NONE},//TOKEN_PRINT
	{NULL,NULL,PREC_NONE},//TOKEN_SEMICOLON
	{string,NULL,PREC_PRIMARY},//TOKEN_STRING
};

static ParseRule getRule(TokenType type){
	return rules[type];
}

static void compileError(Compiler *compiler,char* errDesc){
	fprintf(stderr,"Compiler error , line [%d],%s\n",compiler->previousToken.line,errDesc);
	exit(64);
}

static void parsePrecedence(Compiler* compiler,Precedence precedence){
	advance(compiler);
	ParseRule rule = getRule(compiler->previousToken.type);
	if (rule.prefix == NULL){
		compileError(compiler,"expression prefix is null");
	}
	rule.prefix(compiler);

	while(precedence <= getRule(compiler->currentToken.type).precedence){
		advance(compiler);
		ParseFn infixRule= getRule(compiler->previousToken.type).infix;
		infixRule(compiler);
	}
}

static void consume(Compiler* compiler,TokenType type,char* errDesc){
	if (match(compiler,type)){
		return;
	}else{
		compileError(compiler,errDesc);
	}
}

static  void expression(Compiler* compiler){
	parsePrecedence(compiler,PREC_ASSIGNMENT);

}

static void exprStmt(Compiler* compiler){
	expression(compiler);
	emitByte(compiler,OP_POP);
}

static void printStmt(Compiler* compiler){
	expression(compiler);
	emitByte(compiler,OP_PRINT);
	consume(compiler,TOKEN_SEMICOLON,"expect ';' after print");
}

static void statement(Compiler* compiler){
	if (match(compiler,TOKEN_PRINT)){
		printStmt(compiler);
	}else{
		exprStmt(compiler);
	}
}

static void declaration(Compiler* compiler){
	statement(compiler);
}


ObjFunction* compileHelper(Scanner* scanner){
	Compiler compiler;
	compilerInit(&compiler,scanner);
	Chunk* chunk = getChunk(&compiler);
	advance(&compiler);
	for(;!isEnd(&compiler);){
		declaration(&compiler);
	}
	emitByte(&compiler,OP_NIL);
	emitByte(&compiler,OP_RETURN);
	return compiler.function;
}


ObjFunction* compile(char* source){
	Scanner scanner;
	scannerInit(&scanner,source);
	return compileHelper(&scanner);
}
