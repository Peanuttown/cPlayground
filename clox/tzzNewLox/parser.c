#include "parser.h"
#include "array.h"
#include "token.h"
#include "common.h"
#include <stdio.h>
#include "opcode.h"
#include "value.h"
#include "memory.h"
#include <string.h>
#include "stringObject.h"
#include "token.h"

void statement();
void expression();
static void declaration();
static Token* consume(TokenType,char*);
uint8_t identifierConstant(Token* token);
static bool match(TokenType type);

Token* getTokenByIndex(int index){
	return (Token*)getArray(parser.tokens,index);
}


static void emitByte(OpCode opcode){
	uint8_t code = GET_OPCODE(opcode);
	writeArray(&parser.code,&code);
}

void initParser(Array* tokens){
	parser.tokens =tokens;
	InitArray(&parser.code,uint8_t);
	InitArray(&parser.constant,Value);
}

Token* peek(){
	return getTokenByIndex(parser.current);
}

static Token* advance(){
	parser.current++;
	return getTokenByIndex(parser.current-1);
}

static bool isEnd(){
	return peek()->type == TOKEN_EOF;
}

/*declaration:decVariable
 * 	      |statement
 *
 *decVariable:"var" IDENTIFIER ("=" expression) ;
 *
 * statement: stmtIf|
 *
 * 	     stmtFor|
 *	     stmtPrint
 *	     stmtExpr
 *	   
 *
 * 
 */

static void decVariable(){
	//consume var
	printf("parse decVariable\n");
	advance();
	Token* identifier = consume(TOKEN_IDENTIFIER,"expect identifer after variable declaration");
	int pos = identifierConstant(identifier);
	if (match(TOKEN_EQUAL)){
		expression();
	}else{
		emitByte(OP_NIL);
	}
	consume(TOKEN_SEMICOLON,"expect ';' after statement");
	emitByte(OP_DEFINE_GLOBAL);
	emitByte(pos);

}

static void declaration(){
	Token* token = peek();
	switch (token->type){
		case TOKEN_VAR:{
				       decVariable();
				       break;
			       }
		default:{
				statement();
				break;
			}
	}
}

static void emitConstant(Value value){
	int pos = writeArray(&parser.constant,&value);
	emitByte(OP_CONSTANT);
	emitByte(pos);
	return;
}

uint8_t makeConstant(Value value){
	int pos = writeArray(&parser.constant,&value);
	return pos;
}

static Token* previous(){
	return getTokenByIndex(parser.current-1);
}

static bool match(TokenType type){
	if (peek()->type == type){
		advance();
		return true;
	}
	return false;
}

uint8_t identifierConstant(Token *token){
	return makeConstant(AS_OBJ((Obj*)copyString(token->start,token->length)));
}

static void primary(){
	Token *token = peek();
	printf("here :%d\n",token->type);
	switch (token->type){
		case TOKEN_NUMBER:{
					  //todo value
					  advance();
					  char *buff =(char*)allocMemory(sizeof(char)*(token->length+1));
					  memcpy(buff,token->start,token->length);
					  buff[token->length] = '\0';
					  emitConstant(AS_NUMBER(strtod(buff,NULL)));
					  break;
				  }
		case TOKEN_STRING:{
					  advance();
					  ObjString* s =copyString(token->start,token->length);
					  Obj* obj = (Obj*)(s);
					  emitConstant(AS_OBJ(obj));
					  break;
				  }
		case TOKEN_IDENTIFIER:{
					      advance();
					      int pos = identifierConstant(token);
					      emitByte(OP_GET_GLOBAL);
					      emitByte(pos);
					      break;

				      }
		case TOKEN_LEFT_PARENTHESE:{
						   advance();
						   expression();
						   consume(TOKEN_RIGHT_PARENTHESE,"expect ')'  \n");
						   break;
					   }
		default:{
				fprintf(stdout,"syntax error:unexpect primary %d\n",peek()->type);
				exit(64);
			}
	}

}

static void unary(){
	if (match(TOKEN_BANG) || match(TOKEN_MINUS)){
		unary();
		emitByte(OP_NEGATE);
	}else{
		primary();
	}
}

static void multiplication(){
	unary();
	for( ; match(TOKEN_STAR) || match(TOKEN_SLASH);){
		TokenType operatorType = previous()->type;
		unary();
		uint8_t opcode;
		switch(operatorType){
			case TOKEN_STAR:
				opcode =OP_MUL;
			case TOKEN_SLASH:
				opcode = OP_DIV;
			default:{
					fprintf(stderr,"panic!!! parser.c : mulition() : why occur other operatro????\n");
					exit(64);
				}
		}
		emitByte(opcode);
	}
	return;
}

void addition(){
	multiplication();
	for (;match(TOKEN_PLUS)||match(TOKEN_MINUS);){
		TokenType  operatorType = previous()->type;
		multiplication();
		uint8_t opcode;
		switch(operatorType){
			case TOKEN_PLUS:
				opcode =OP_ADD;
				break;
			case TOKEN_MINUS:
				opcode =OP_SUB;
				break;
			default:{
					fprintf(stderr,"panic!!! parser.c : addition() : why occur other operatro????\n");
					exit(64);
				}
		}
		emitByte(opcode);
	}
	return;
}

/*
 *expression: assignment ;
 *assignment: (IDENTIFER ("=" assignment)*) | equality ;
 *equality:comparison ("=="|"!=" comparison)*  ;
 *comparsion:addition ("<"|"<="|">"|">=" addition)*  ;
 *addition: multiplication ("+"|"-" multiplication;)* ;
 *multiplication: unary ("*"|"/" unary)*   ;
 *unary: ("!"|"-" unary) | primary;
 *primary: NUMBER | IDETIFIER | STRING | "(" expression ")"  ;
 *
 *
 * */

void comparsion(){
	addition();
	for (;match(TOKEN_LESS)||match(TOKEN_LESS_EQUAL)||match(TOKEN_GREATER)||match(TOKEN_GREATER_EQUAL);){
		Token* operator = previous();
		uint8_t opcode;
		if (operator->type == TOKEN_LESS){
			opcode =OP_LESS;
		}else if (operator->type == TOKEN_LESS_EQUAL){
			opcode =OP_LESS_EQUAL;
		}else if(operator->type == TOKEN_GREATER){
			opcode =OP_GREATER;
		}else if (operator->type == TOKEN_GREATER_EQUAL){
			opcode =OP_GREATER_EQUAL;
		}else{
			fprintf(stderr,"parse error ,comparsion() unexpect operator %d\n",operator->type);
			exit(64);
		}
		addition();
		emitByte(opcode);
	}

}

void equality(){
	comparsion();
	for(;match(TOKEN_EQUAL_EQUAL)||match(TOKEN_BANG_EQUAL);){
		Token* operator = previous();
		uint8_t opcode =0;
		if (operator->type == TOKEN_EQUAL_EQUAL){
			opcode = OP_EQUAL_EQUAL;
		}else if (operator->type == TOKEN_BANG_EQUAL){
			opcode = OP_BANG_EQUAL;
		}else{
			fprintf(stderr,"parser error,equality() operator type unexpect %d\n",operator->type);
			exit(64);
		}
		comparsion();
		emitByte(opcode);
	}
}

void assignment(){
	equality();
	for (;match(TOKEN_EQUAL);){
		((uint8_t*)parser.code.values)[parser.code.count-2] =OP_SET_GLOBAL;
		printf("parse assign\n");
		assignment();
		emitByte(OP_ASSIGN);
	}
}

void expression(){
	assignment();
}

static Token* consume(TokenType type,char* errDesc){
	if (match(type)){
		return previous();
	}
	Token* token = getTokenByIndex(parser.current);
	fprintf(stderr,"consume err at line %d:%s\n",token->line,errDesc);
	exit(64);
}

//return value is the offset which patch is
int emitJump(uint8_t opcode){
	emitByte(opcode);
	emitByte(0xff);
	emitByte(0xff);
	return parser.code.count-2;
}


void patchJump(int offset){
	int jump = parser.code.count -offset - 2;

	if (jump >UINT16_MAX){
		fprintf(stderr,"compile error ,too much code to jump at line %d\n",getTokenByIndex(parser.current)->line);
		exit(64);
	}
	uint8_t mostSignificant = (offset >>8);
	uint8_t leastSignificant =offset;
	writeArrayAt(&parser.code,&mostSignificant,offset);
	writeArrayAt(&parser.code,&leastSignificant,offset+1);
}

static void stmtIf(){
	consume(TOKEN_LEFT_PARENTHESE,"expect '(' after if statement");
	expression();
	consume(TOKEN_RIGHT_PARENTHESE,"expect ')' after if statement");
	//emit a then offset
	int thenJump = emitJump(OP_JUMP_IF_FALSE);
	emitJump(OP_POP);
	statement();//if stmt
	int elseJump = emitJump(OP_JUMP);
	patchJump(thenJump);
	emitJump(OP_POP);
	if (match(TOKEN_ELSE)){
		statement();
	}
	patchJump(elseJump);
	return ;
}

void stmtPrint(){
	advance();//consume print
	printf("stmtPrint:%d\n",peek()->type);
	expression();
	consume(TOKEN_SEMICOLON,"expect ; after statement");
	emitByte(OP_PRINT);
}

void stmtExpr(){
	expression();
	consume(TOKEN_SEMICOLON,"expect ';' after statement");
}

void stmtReturn(){
	advance();//consume return
	emitByte(OP_RETURN);
	//todo emit return value
}

void statement(){
	Token* token = peek();
	switch (token->type){
		case TOKEN_PRINT :{
					  printf("parse print\n");
					  stmtPrint();
					  break;
				  }
		case TOKEN_IF:{
				      stmtIf();
				      break;
			      }
		case TOKEN_RETURN:{
					  stmtReturn();
					  break;
				  }
		default:{
				stmtExpr();
				break;
			}
	}
}

void parse(){
	for (;!isEnd();){
		declaration();
	}
}
