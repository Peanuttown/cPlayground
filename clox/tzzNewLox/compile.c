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
	Table* strings;
}Compiler;

static void statement(Compiler* compiler);
static void declaration(Compiler* compiler);

typedef void(*ParseFn)(Compiler* compiler);

typedef struct{
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;
}ParseRule;



static void compilerInit(Compiler* compiler,Scanner* scanner,Table* strings){
	compiler->scanner = scanner;
	compiler->function =newFunction();
	compiler->strings = strings;

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


static void expression(Compiler* compiler);
static uint8_t makeConstant(Compiler* compiler,Value value);

static void string(Compiler* compiler){
	Token stringToken = compiler->previousToken;
	ObjString* str = copyString(compiler->strings,&stringToken.name[1],stringToken.length-2);
	emitConstant(compiler,OBJ_VAL(str));
}

static void variable(Compiler* compiler){
	Token varName = compiler->previousToken;
	if (match(compiler,TOKEN_EQUAL)){//set global;
		expression(compiler);
		int offset =makeConstant(compiler,OBJ_VAL(copyString(compiler->strings,varName.name,varName.length)));
		emitByte(compiler,OP_SET_GLOBAL);
		emitByte(compiler,offset);
	}else{
		int offset =makeConstant(compiler,OBJ_VAL(copyString(compiler->strings,varName.name,varName.length)));
		emitByte(compiler,OP_GET_GLOBAL);
		emitByte(compiler,offset);
	}
}

const ParseRule rules[]={
	{NULL,NULL,PREC_NONE},//TOKEN_EOF,
	{variable,NULL,PREC_NONE},//TOKEN_IDENTIFIER, 
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
	{NULL,NULL,PREC_NONE},//TOKEN_VAR
	{NULL,NULL,PREC_NONE},//TOKEN_LEFT_BRACE
	{NULL,NULL,PREC_NONE},//TOKEN_RIGHT_BRACE
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
	consume(compiler,TOKEN_SEMICOLON,"expect ';' after statement");
}

static void printStmt(Compiler* compiler){
	expression(compiler);
	emitByte(compiler,OP_PRINT);
	consume(compiler,TOKEN_SEMICOLON,"expect ';' after print");
}


static void beginScope(Compiler* compiler){

}

static void endScope(Compiler* compiler){

}

static Token peek(Compiler* compiler){
	return compiler->previousToken;
}

static void block(Compiler* compiler){
	beginScope(compiler);
	for (;isEnd(compiler)&&peek(compiler).type!=TOKEN_RIGHT_BRACE;){
		declaration(compiler);
	}
	endScope(compiler);
}

static void statement(Compiler* compiler){
	if (match(compiler,TOKEN_PRINT)){
		printStmt(compiler);
	}else if(match(compiler,TOKEN_LEFT_BRACE)){
		block(compiler);
	}else{
		exprStmt(compiler);
	}
}

static uint8_t makeConstant(Compiler* compiler,Value value){
	return arrayWrite(&compiler->function->chunk.constants,&value);
}

static uint8_t parseVariable(Compiler* compiler){
	consume(compiler,TOKEN_IDENTIFIER,"Expect variable name");
	Token idetifier = compiler->previousToken;
	return makeConstant(compiler,OBJ_VAL(copyString(compiler->strings,idetifier.name,idetifier.length)));
}

static void defineVariable(Compiler* compiler,uint8_t offset){
	emitByte(compiler,OP_DEFINE_GLOBAL);
	emitByte(compiler,offset);
}

static void varDecl(Compiler* compiler){
	//parse variable name ,push the name to constants
	uint8_t identiferOffset = parseVariable(compiler);

	//parse expression;
	if (match(compiler,TOKEN_EQUAL)){
		expression(compiler);
	}else{
		emitByte(compiler,OP_NIL);
	}
	defineVariable(compiler,identiferOffset);
	consume(compiler,TOKEN_SEMICOLON,"Expect ';' after statement");
}


static void declaration(Compiler* compiler){
	if (match(compiler,TOKEN_VAR)){
		varDecl(compiler);
	}else{
		statement(compiler);
	}
}


ObjFunction* compileHelper(Scanner* scanner,Table* strings){
	Compiler compiler;
	compilerInit(&compiler,scanner,strings);
	Chunk* chunk = getChunk(&compiler);
	advance(&compiler);
	for(;!isEnd(&compiler);){
		declaration(&compiler);
	}
	emitByte(&compiler,OP_NIL);
	emitByte(&compiler,OP_RETURN);
	return compiler.function;
}


ObjFunction* compile(char* source,Table* strings){
	Scanner scanner;
	scannerInit(&scanner,source);
	return compileHelper(&scanner,strings);
}
