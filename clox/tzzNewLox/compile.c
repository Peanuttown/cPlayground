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
	Token name;
	int depth;
}Local;

typedef struct{
	Token previousToken;
	Token currentToken;
}Parser;

typedef struct{
	ObjFunction* function;
	Scanner* scanner;
	Parser* parser;
	Table* strings;
	int localCount;
	int localDepth;
	Local locals[UINT8_MAX];
}Compiler;

ObjFunction* compileHelper(Compiler *compiler);
static void statement(Compiler* compiler);
static void declaration(Compiler* compiler);
static bool isInScope(Compiler* compiler);
static bool tokenNameEqual(Token a,Token b);
static bool check(Compiler* compiler,TokenType type);
static void consume(Compiler* compiler,TokenType type,char* errDesc);

static Token getPreviousToken(Compiler* compiler){
	return compiler->parser->previousToken;
}

static Token getCurrentToken(Compiler* compiler){
	return compiler->parser->currentToken;
}


typedef void(*ParseFn)(Compiler* compiler);

typedef struct{
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;
}ParseRule;



static void compilerInit(Compiler* compiler,Scanner* scanner,Table* strings,Parser* parser){
	compiler->scanner = scanner;
	compiler->parser = parser;
	compiler->function =newFunction();
	compiler->strings = strings;
	compiler->localDepth = 0;
	compiler->localCount = 0;
	Local local;
	local.depth=0;
	Token name;
	name.name = "";
	local.name=name;
	compiler->locals[compiler->localCount++] =local;
}

static Chunk* getChunk(Compiler* compiler){
	return &compiler->function->chunk;
}

static bool isEnd(Compiler* compiler){
	return compiler->parser->currentToken.type == TOKEN_EOF;
}

static void advance(Compiler* compiler){
	compiler->parser->previousToken = compiler->parser->currentToken;
	compiler->parser->currentToken = scanToken(compiler->scanner);
}

static bool match(Compiler* compiler,TokenType type){
	if (getCurrentToken(compiler).type == type){
		advance(compiler);
		return true;
	}
	return false;
}

void emitByte(Compiler* compiler,uint8_t code){
	arrayWrite(&getChunk(compiler)->codes,&code);
	arrayWrite(&getChunk(compiler)->lines,&compiler->parser->previousToken.line);;
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
	Token numberToken = getPreviousToken(compiler);
	double value = strtod(numberToken.name,NULL);
	emitConstant(compiler,NUMBER_VAL(value));
}

void binary(Compiler* compiler){
	Token operator = getPreviousToken(compiler);

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
	Token stringToken = getPreviousToken(compiler);
	ObjString* str = copyString(compiler->strings,&stringToken.name[1],stringToken.length-2);
	emitConstant(compiler,OBJ_VAL(str));
}

static int findLocalIndex(Compiler* compiler,Token name){
	for (int index =compiler->localCount-1;index >=0;index--){
		Local* local = &compiler->locals[index];
		if (tokenNameEqual(name,local->name)&&&local->depth>0){
			return index;
		}
	}
	return -1;
}

static void variable(Compiler* compiler){
	Token varName = getPreviousToken(compiler);
	int offset =0;
	uint8_t opcode;
	offset = findLocalIndex(compiler,varName);
	if (isInScope(compiler)&&offset != -1)  { 
		if (match(compiler,TOKEN_EQUAL)){
			expression(compiler);
			opcode = OP_SET_LOCAL;
		}else{
			opcode =OP_GET_LOCAL;
		}
	}else{
		if (match(compiler,TOKEN_EQUAL)){//set global;
			expression(compiler);
			opcode = OP_SET_GLOBAL;
		}else{
			opcode = OP_GET_GLOBAL;
		}
		offset =makeConstant(compiler,OBJ_VAL(copyString(compiler->strings,varName.name,varName.length)));
	}
	emitByte(compiler,opcode);
	emitByte(compiler,offset);
}

static int argumentList(Compiler* compiler){
	int argCount = 0;
	for(;!isEnd(compiler)&&!check(compiler,TOKEN_RIGHT_PARENTHESE);){
		expression(compiler);
		argCount++;
		if (match(compiler,TOKEN_DOT)){
			continue;
		}
		break;
	}
	consume(compiler,TOKEN_RIGHT_PARENTHESE,"expect ')'");
	return argCount;
}

static void call(Compiler* compiler){
	int argCount = argumentList(compiler);
	emitByte(compiler,OP_CALL);
	emitByte(compiler,argCount);
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
	{NULL,call,PREC_CALL},//TOKEN_LEFT_PARENTHESE
	{NULL,NULL,PREC_NONE},//TOKEN_RIGHT_PARENTHESE
	{NULL,NULL,PREC_NONE},//TOKEN_DOT
};

static ParseRule getRule(TokenType type){
	return rules[type];
}

static void compileError(Compiler *compiler,char* errDesc){
	fprintf(stderr,"Compiler error , line [%d],%s\n",compiler->parser->previousToken.line,errDesc);
	exit(64);
}

static void parsePrecedence(Compiler* compiler,Precedence precedence){
	advance(compiler);
	ParseRule rule = getRule(getPreviousToken(compiler).type);
	if (rule.prefix == NULL){
		compileError(compiler,"expression prefix is null");
	}
	rule.prefix(compiler);

	while(precedence <= getRule(getCurrentToken(compiler).type).precedence){
		advance(compiler);
		ParseFn infixRule= getRule(getPreviousToken(compiler).type).infix;
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
	compiler->localDepth++;
}

static void endScope(Compiler* compiler){
	for (;compiler->localCount>0;){
		Local* local = &compiler->locals[compiler->localCount-1];
		if (local->depth >0){
			if (local->depth == compiler->localDepth){
				compiler->localCount--;
				emitByte(compiler,OP_POP);
				continue;
			}
		}
		break;
	}
	compiler->localDepth--;
}

static Token peek(Compiler* compiler){
	return getCurrentToken(compiler);
}

static void block(Compiler* compiler){
	for (;!isEnd(compiler)&&peek(compiler).type!=TOKEN_RIGHT_BRACE;){
		declaration(compiler);
	}
	consume(compiler,TOKEN_RIGHT_BRACE,"expect '}' ");
}

static void statement(Compiler* compiler){
	if (match(compiler,TOKEN_PRINT)){
		printStmt(compiler);
	}else if(match(compiler,TOKEN_LEFT_BRACE)){
		beginScope(compiler);
		block(compiler);
		endScope(compiler);
	}else{
		exprStmt(compiler);
	}
}

static uint8_t makeConstant(Compiler* compiler,Value value){
	return arrayWrite(&compiler->function->chunk.constants,&value);
}

static bool isInScope(Compiler *compiler){
	return compiler->localDepth > 0;
}

static void addLocal(Compiler* compiler,Token name){
	Local* local = &compiler->locals[compiler->localCount];
	local->name = name;
	local->depth = compiler->localDepth;
	compiler->localCount++;
}

static bool tokenNameEqual(Token a,Token b){
	return (a.length==b.length &&memcmp(a.name,b.name,a.length)==0);
}

static bool checkLocalRedeclare(Compiler* compiler,Token name){
	for (int index =compiler->localCount-1;index>=0;index--){
		Local* local = &compiler->locals[index];
		if ((local->depth) < compiler->localDepth){
			return false;
		}
		if (tokenNameEqual(name,local->name)){
			return true;
		}
	}
	return false;

}

static uint8_t parseVariable(Compiler* compiler){
	consume(compiler,TOKEN_IDENTIFIER,"Expect variable name");
	if (isInScope(compiler)) {
		//check variable redecalrae
		if (checkLocalRedeclare(compiler,getPreviousToken(compiler))){
			fprintf(stderr,"varaiable redeclare\n");
			exit(64);
		}
		addLocal(compiler,getPreviousToken(compiler));
		return 0;
	}
	Token idetifier = getPreviousToken(compiler);
	return makeConstant(compiler,OBJ_VAL(copyString(compiler->strings,idetifier.name,idetifier.length)));
}

static void defineVariable(Compiler* compiler,uint8_t offset){
	if (isInScope(compiler)){
		return;
	}else{
		emitByte(compiler,OP_DEFINE_GLOBAL);
		emitByte(compiler,offset);
	}
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

static bool check(Compiler* compiler,TokenType type){
	return peek(compiler).type == type;
}

static ObjFunction* function(Compiler* compiler){
	//parse args
	Compiler compilerNext;
	compilerInit(&compilerNext,compiler->scanner,compiler->strings,compiler->parser);;
	Token funcName = getPreviousToken(&compilerNext);
	consume(&compilerNext,TOKEN_LEFT_PARENTHESE,"expect '(' after function name");
	int arity =0;
	beginScope(&compilerNext);
	for(;!isEnd(&compilerNext)&&!check(&compilerNext,TOKEN_RIGHT_PARENTHESE);){
		arity ++;
		int varibleOffset = parseVariable(&compilerNext);
		defineVariable(&compilerNext,varibleOffset);
		if (match(&compilerNext,TOKEN_DOT)){
			continue;
		}
	}
	consume(&compilerNext,TOKEN_RIGHT_PARENTHESE,"Expect ')' after args");
	consume(&compilerNext,TOKEN_LEFT_BRACE,"Expect '{'");
	block(&compilerNext);
	ObjFunction* function = compilerNext.function;
	function->name = copyCString(funcName.name,funcName.length); 
	function->arity =arity;
	emitByte(&compilerNext,OP_NIL);
	emitByte(&compilerNext,OP_RETURN);
	endScope(&compilerNext);
	return function;
}


static void funcDecl(Compiler* compiler){
	int funcNameOffset = parseVariable(compiler);
	ObjFunction* func = function(compiler);
	emitConstant(compiler,OBJ_VAL(func));
	defineVariable(compiler,funcNameOffset);
}

static void declaration(Compiler* compiler){
	if (match(compiler,TOKEN_VAR)){
		varDecl(compiler);
	}else if(match(compiler,TOKEN_FUNC)){
		funcDecl(compiler);
	}else{
		statement(compiler);
	}
}


ObjFunction* compileHelper(Compiler *compiler){
	Chunk* chunk = getChunk(compiler);
	advance(compiler);
	for(;!isEnd(compiler);){
		declaration(compiler);
	}
	emitByte(compiler,OP_NIL);
	emitByte(compiler,OP_RETURN);
	return compiler->function;
}


ObjFunction* compile(char* source,Table* strings){
	Scanner scanner;
	scannerInit(&scanner,source);
	Parser parser;
	Compiler compiler;
	compilerInit(&compiler,&scanner,strings,&parser);
	return compileHelper(&compiler);
}
