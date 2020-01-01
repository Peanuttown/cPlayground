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
	ObjFunction* function;
	Scanner* scanner;
	Token currentToken;
	Token previousToken;
	Table* strings;
	int localCount;
	int localDepth;
	Local locals[UINT8_MAX];
}Compiler;

static void statement(Compiler* compiler);
static void declaration(Compiler* compiler);
static bool isInScope(Compiler* compiler);
static bool tokenNameEqual(Token a,Token b);

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
	Token varName = compiler->previousToken;
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
	compiler->localDepth++;
}

static void endScope(Compiler* compiler){
	for (;compiler->localCount>0;){
		Local* local = &compiler->locals[compiler->localCount-1];
		if (local->depth >0){
			if (local->depth == compiler->localDepth){
				compiler->localCount--;
				emitByte(compiler,OP_POP);
			}
			break;
		}
	}
	compiler->localDepth--;
}

static Token peek(Compiler* compiler){
	return compiler->currentToken;
}

static void block(Compiler* compiler){
	beginScope(compiler);
	for (;!isEnd(compiler)&&peek(compiler).type!=TOKEN_RIGHT_BRACE;){
		declaration(compiler);
	}
	consume(compiler,TOKEN_RIGHT_BRACE,"expect '}' ");
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
		if (checkLocalRedeclare(compiler,compiler->previousToken)){
			fprintf(stderr,"varaiable redeclare\n");
			exit(64);
		}
		addLocal(compiler,compiler->previousToken);
		return 0;
	}
	Token idetifier = compiler->previousToken;
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
