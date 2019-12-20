#include "compiler.h"
#include "scanner.h"
#include <stdio.h>
#include "chunk.h"
#include <stdlib.h>
#include "string.h"
#include "chunk.h"
#include "object.h"
#include "value.h"

#ifdef DEBUG_PRINT_CODE
#include "debug.h"
#endif

/* grammar
 * statement: exprStmt 
 * 		| forStmt
 * 		| ifStmt
 * 		| printStmt
 * 		| returnStmt
 * 		| whileStmt
 * 		| block;
 *declaration: classDecl
 *		| funcDecl
 *		|varDecl
 *		|statement;
 *
 *
 *
 *
 * */

static void binary(bool canAssign);
static void number(bool canAssign);
static void unary(bool canAssign);
static void group(bool canAssign);
static void literal(bool canAssign);
static void string(bool canAssign);
static void emitConstant(Value value);
static void variable(bool canAssign);
static uint8_t identifierConstant(Token* token);
static bool match(TokenType type);
static void declaration();
static bool identifierEquals(Token* a,Token* b);
static void statement();
static void and(bool canAssign);
static void or(bool canAssign);


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


typedef void (*ParseFn)(bool canAssign);

typedef struct{
	ParseFn prefix;
	ParseFn infix;
	Precedence precedence;

}ParseRule;

typedef struct{
	Token name;
	int depth;
}Local;


typedef struct{
	Local locals[UINT8_COUNT];
	int localCount;
	int scopeDepth;

}Compiler;

Compiler* current = NULL;

void initCompiler(Compiler* compiler){
	compiler->localCount = 0;
	compiler->scopeDepth =0;
	current = compiler;
}

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
	{ unary,     NULL,    PREC_NONE },       // TOKEN_BANG            
	{ NULL,     binary,    PREC_EQUALITY },       // TOKEN_BANG_EQUAL      
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_EQUAL           
	{ NULL,     binary,    PREC_EQUALITY },       // TOKEN_EQUAL_EQUAL     
	{ NULL,     binary,    PREC_COMPARSION },       // TOKEN_GREATER         
	{ NULL,     binary,    PREC_COMPARSION },       // TOKEN_GREATER_EQUAL   
	{ NULL,     binary,    PREC_COMPARSION },       // TOKEN_LESS            
	{ NULL,     binary,    PREC_COMPARSION },       // TOKEN_LESS_EQUAL      
	{ variable,     NULL,    PREC_NONE },       // TOKEN_IDENTIFIER      
	{ string,     NULL,    PREC_NONE },       // TOKEN_STRING          
	{ number,   NULL,    PREC_NONE },       // TOKEN_NUMBER          
	{ NULL, and,    PREC_AND},       // TOKEN_AND             
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_CLASS           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_ELSE            
	{ literal,     NULL,    PREC_NONE },       // TOKEN_FALSE           
	{ NULL, NULL,    PREC_NONE},       // TOKEN_FOR             
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_FUN             
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_IF              
	{ literal,     NULL,    PREC_NONE },       // TOKEN_NIL             
	{ NULL, or,    PREC_OR},       // TOKEN_OR              
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_PRINT           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_RETURN          
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_SUPER           
	{ NULL,     NULL,    PREC_NONE },       // TOKEN_THIS            
	{ literal,     NULL,    PREC_NONE },       // TOKEN_TRUE            
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


static void literal(bool canAssign){
	switch(parser.previous.type){
		case TOKEN_FALSE: {
					  emitByte(OP_FALSE);
					  break;
				  }
		case TOKEN_TRUE:{
					emitByte(OP_TRUE);
					break;
				}
		case TOKEN_NIL:{
				       emitByte(OP_NIL);
				       break;
			       } 
		default:{
				fprintf(stderr,"literal error ,unexpect literal type %d\n",parser.previous.type);
				exit(64);
			}

	}
}


static void string(bool canAssign){
	emitConstant(OBJ_VAL(copyString(parser.previous.start+1,parser.previous.length-2)));
}

static void emitBytes(uint8_t a,uint8_t b){
	emitByte(a);
	emitByte(b);
}
static int resolveLocal(Compiler* compiler,Token* name){
	for(int i = current->localCount-1;i>=0;i--){
		Local* local = &current->locals[i];
		if (identifierEquals(&local->name,name)){
			return i;
		}
	}
	return -1;
}

static void namedVariable(Token* token,bool canAssign){
	//uint8_t arg = identifierConstant(token);
	
	int arg =  resolveLocal(current,token);
	if (arg == -1){//global variable
		int arg = identifierConstant(token);
		if (match(TOKEN_EQUAL)&&canAssign){
			expression();
			emitBytes(OP_SET_GLOBAL,arg);
		}else{
			emitBytes(OP_GET_GLOBAL,arg);
		}
	}else{
		if (match(TOKEN_EQUAL)&&canAssign){
			expression();
			emitBytes(OP_SET_LOCAL,arg);
		}else{
			emitBytes(OP_GET_LOCAL,arg);
		}

	}
}

static void variable(bool canAssign){
	namedVariable(&parser.previous,canAssign);
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

static void number(bool canAssign){
	double  value=strtod(parser.previous.start,NULL);
	emitConstant(NUMBER_VAL(value));
}

static void group(bool canAssign){
	expression();
	consume(TOKEN_RIGHT_PAREN,"Expect ')' after expression");
}


static void binary(bool canAssign){
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
		case TOKEN_EQUAL_EQUAL:{
					       emitByte(OP_EQUAL);
					       break;
				       }
		case TOKEN_BANG_EQUAL:{
					      emitByte(OP_EQUAL);
					      emitByte(OP_NOT);
					      break;
				      }
		case TOKEN_GREATER:{
					   emitByte(OP_GREATER);
					   break;
				   }
		case TOKEN_GREATER_EQUAL:{
						 emitByte(OP_LESS);
						 emitByte(OP_NOT);
						 break;
					 }
		case TOKEN_LESS:{
					emitByte(OP_LESS);
					break;
				}
		case TOKEN_LESS_EQUAL:{
					      emitByte(OP_GREATER);
					      emitByte(OP_NOT);
					      break;
				      }
		default:{
				fprintf(stderr,"compile bug!!!! unexpected binary operatorType %d\n",operatorType);
				exit(64);
			}
	}


}

static void unary(bool canAssign){
	TokenType operatorType = parser.previous.type;

	//compile the operand
	parsePrecedence(PREC_UNARY);

	switch(operatorType){
		case TOKEN_MINUS:{
					 emitByte(OP_NEGATE); 
					 break;
				 }
		case TOKEN_BANG:{
					emitByte(OP_NOT);
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
	bool canAssign =  precedence <= PREC_ASSIGNMENT;
	prefixRule(canAssign);

	while(precedence <= getRule(parser.current.type)->precedence){
		advance();
		ParseFn infixRule = getRule(parser.previous.type)->infix;
		infixRule(canAssign);
	}
}

static void expression(){
	parsePrecedence(PREC_ASSIGNMENT);
}

static bool check(TokenType type){
	return  parser.current.type == type;
}

static bool match(TokenType type){
	if (check(type)){
		advance();
		return true;
	}
	return false;
}

static void printStmt(){
	expression();
	consume(TOKEN_SEMICOLON,"Expect ';' after value.");
	emitByte(OP_PRINT);
}

static void expressionStmt(){
	expression();
	consume(TOKEN_SEMICOLON,"Expect ';' after expression..");
	emitByte(OP_POP);
}


static void block(){
	while(!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF)){
		declaration();
	}
	consume(TOKEN_RIGHT_BRACE,"Expect '}' after block");
}

static void beginScope(){
	current->scopeDepth++;
}
static void endScope(){
	current->scopeDepth--;

	while(current->localCount >0 && current->locals[current->localCount-1].depth > current->scopeDepth){
		emitByte(OP_POP);
		current->localCount--;
	}

}

static int emitJump(uint8_t op){
	emitByte(op);
	emitByte(0xff);
	emitByte(0xff);
	return currentChunk()->count -2;
}

static void patchJump(int offset){
	int jump  = currentChunk()->count - offset -2;
	if (jump > UINT16_MAX){
		error("Too much code to jump over");
	}

	currentChunk()->code[offset] =(jump>>8)&0xff; 
	currentChunk()->code[offset+1] =(jump)&0xff; 

}

static void or(bool canAssign){
	int elseJump = emitJump(OP_JUMP_IF_FALSE);
	int endJump = emitJump(OP_JUMP);
	patchJump(elseJump);
	emitByte(OP_POP);
	parsePrecedence(PREC_OR);
	patchJump(endJump);

}

static void and(bool canAssign){
	int  endJump = emitJump(OP_JUMP_IF_FALSE);
	emitByte(OP_POP);
	parsePrecedence(PREC_AND);
	patchJump(endJump);

}

static void ifStmt(){
	//parse condition
	consume(TOKEN_LEFT_PAREN,"Expect '(' after if");
	expression();//condition has put to code array
	consume(TOKEN_RIGHT_PAREN,"Expect ')' after if");
	//parse body
	int thenJump = emitJump(OP_JUMP_IF_FALSE);
	emitByte(OP_POP);
	statement(); //how to resolve if jump???
	int elseJump = emitJump(OP_JUMP);
	patchJump(thenJump);
	emitByte(OP_POP);
	if (match(TOKEN_ELSE)) statement();
	patchJump(elseJump);

}

static void emitLoop(int loopStart){
	emitByte(OP_LOOP);
	int offset = currentChunk()->count - loopStart + 2;
	if (offset > UINT16_MAX) error("Loop body too large");
	emitByte((offset>>8)&0xff);
	emitByte((offset)&0xff);

}

static void whileStmt(){
	int loopStart = currentChunk()->count;
	consume(TOKEN_LEFT_PAREN,"Expect '(' after while");
	expression();
	consume(TOKEN_RIGHT_PAREN,"Expect '(' after while");

	int exitJump = emitJump(OP_JUMP_IF_FALSE);
	emitByte(OP_POP);
	statement();
	emitLoop(loopStart);
	patchJump(exitJump);
	emitByte(OP_POP);

}


static void statement(){
	if (match(TOKEN_PRINT)){
		printStmt();

	}else if (match(TOKEN_LEFT_BRACE)){
		beginScope();
		block();
		endScope();
	}else if (match(TOKEN_IF)){
		ifStmt();
	}else if(match(TOKEN_WHILE)){
		whileStmt();
	}
	else{
		expressionStmt();
	}
}

static uint8_t identifierConstant(Token* token){
	return makeConstant(OBJ_VAL(copyString(token->start,token->length)));
}

static void addLocal(Token name){
	if (current->localCount == UINT8_COUNT){
		error("Too many local variables in function.");
		return;
	}
	Local* local = &current->locals[current->localCount++];
	local->name = name;
	local->depth = current->scopeDepth;
}

static bool identifierEquals(Token* a,Token* b){
	if (a->length != b->length) return false; 
	printf("a:%.*s\n",a->length,a->start);
	printf("b:%.*s\n",b->length,b->start);
	return memcmp(a->start,b->start,a->length)==0;
}

static void decalreVariable(){
    if (current->scopeDepth == 0 ) return;

    Token* name = &parser.previous;

    for (int i = current->localCount - 1; i>= 0;i--){
	    Local* local = &current->locals[i];
	    if (local->depth != -1 && local->depth < current->scopeDepth){
		    break;
	    }

	    if (identifierEquals(&local->name,name)){
		    error("Variable has decalred");
	    }
    }

    addLocal(*name);
}

static uint8_t parseVariable(char* errMsg){
	consume(TOKEN_IDENTIFIER,errMsg);

	decalreVariable();
	if (current->scopeDepth > 0) return 0;

	return identifierConstant(&parser.previous);
}

static void defineVariable(uint8_t global){
	if (current->scopeDepth > 0 ) return;
	emitByte(OP_DEFINE_GLOBAL);
	emitByte(global);
}

static void varDecl(){
	uint8_t global = parseVariable("Expect variable name");

	if (match(TOKEN_EQUAL)){
		expression();
	}else{
		emitByte(OP_NIL);
	}
	consume(TOKEN_SEMICOLON,"expect ';' after declaration");
	 defineVariable(global);
}

static void declaration(){
	if (match(TOKEN_VAR)){
		varDecl();
	}else{
		statement();
	}
}

static void synchronize(){
	parser.panicMode = false;
	while(parser.current.type != TOKEN_EOF){
		if (parser.previous.type == TOKEN_SEMICOLON) return;

		switch(parser.current.type){
			case TOKEN_CLASS:                                 
			case TOKEN_FUN:                                   
			case TOKEN_VAR:                                   
			case TOKEN_FOR:                                   
			case TOKEN_IF:                                    
			case TOKEN_WHILE:                                 
			case TOKEN_PRINT:                                 
			case TOKEN_RETURN:                                
				return;                                         

			default:                                          
				// Do nothing.                                  
				;

		}
		advance();
	}
}

bool compile(char* source,Chunk* chunk){
	initScanner(source);
	Compiler compiler;
	initCompiler(&compiler);
	compilingChunk = chunk;
	parser.hadError  =false;
	parser.panicMode = false;
	advance();

	while(!match(TOKEN_EOF)){
		declaration();

		if (parser.panicMode) synchronize();
	}
	consume(TOKEN_EOF,"Expect enf of expression.");
	endCompile();
	printf("comile over\n");


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


