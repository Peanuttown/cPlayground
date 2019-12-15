#include "scanner.h"
#include "array.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"


void initScanner(char* source){
	scanner.source=source;
	scanner.current = 0;//remember to init
	scanner.start = 0;
	scanner.line= 1;
	InitArray(&scanner.tokens,Token);
}

static bool isEnd(){
	return scanner.source[scanner.current] =='\0';
}

static char advance(){
	scanner.current++;
	return scanner.source[scanner.current-1];
}

static Token* getTokenByIndex(int index){
		return (Token*)getArray(&scanner.tokens,index);
}

void addToken(TokenType type){
	Token token;
	token.type=type;
	token.line=scanner.line;
	token.start = &scanner.source[scanner.start];
	token.length = scanner.current - scanner.start;
	writeArray(&scanner.tokens,&token);
}

static char peek(){
	return scanner.source[scanner.current];
}

static bool match(char expect){
	if (peek() == expect){
		advance();
		return true;
	}
	return false;
}

//jump white space
static void skipWhitespace(){
	for (;!isEnd();){
		switch (peek()){
			case '\r':
				advance();
				break;
			case '\n':
				advance();
				scanner.line++;
				break;
			case '\t':
				advance();
				break;
			case ' ':
				advance();
				break;
			default:
				return;
		}
	}
}

static bool isDigit(char c){
	return c>='0' && c<='9';
}

static bool isAlpha(char c){
	return (c>='a' && c <='z') || (c >='A' && c<='Z');
}

static bool isAlphaNumeric(char c){
	return isDigit(c) || isAlpha(c);
}

static bool compareString(char* toCompare){
	return memcmp(toCompare,&scanner.source[scanner.start],scanner.current-scanner.start)==0;

}

static void identifier(){
	for (;isAlphaNumeric(peek());){
		advance();
	}
	//check keyword
	if (compareString("print")){
		addToken(TOKEN_PRINT);
		return;
	}else if (compareString("var")){
		addToken(TOKEN_VAR);
		return;
	}else if (compareString("if")){
		addToken(TOKEN_IF);
		return;
	}else if(compareString("for")){
		addToken(TOKEN_FOR);
		return;

	}else if (compareString("else")){
		addToken(TOKEN_ELSE);
		return;
	}else if (compareString("return")){
		addToken(TOKEN_RETURN);
		return;
	}else{
		addToken(TOKEN_IDENTIFIER);
		return;
	}
}

static char peekNext(){
	if (isEnd()){
		return '\0';
	}
	return scanner.source[scanner.current+1];
}

static void number(){
	//scan until not digit
	for (;isDigit(peek());){
		advance();
	}
	if (peek()=='.'&&isDigit(peekNext())){
		advance();
		for(;isDigit(peek());){
			advance();
		}
	}
	addToken(TOKEN_NUMBER);
}

static void consume(char c ,char* errDesc){
	if (match(c)){
		return;
	}
	fprintf(stderr,"scan error at line %d:%s\n",scanner.line,errDesc);
	return;
}

static void string(){
	for (;!isEnd()&&peek()!='"';){
		advance();
	}
	if (isEnd()){
		consume('"',"unterminated string");
	}
	scanner.start++;
	addToken(TOKEN_STRING);
	advance();
}

bool Scan(){
	for (;!isEnd();){
		//skip white space
		skipWhitespace();
		//consume one char
		scanner.start = scanner.current;
		if (isEnd()){
			break;
		}
		char c = advance();
		if (isDigit(c)){
			number();
			continue;
		}

		if (isAlpha(c)){
			identifier();
			continue;
		}

		switch (c){
			case '+':
				addToken(TOKEN_PLUS);
				break;
			case '-':
				addToken(TOKEN_MINUS);
				break;
			case '*':
				addToken(TOKEN_STAR);
				break;
			case '/':
				//todo may be remark
				addToken(TOKEN_SLASH);
				break;
			case '.':
				addToken(TOKEN_DOT);
				break;
			case ';':
				addToken(TOKEN_SEMICOLON);
				break;
			case '=':
				addToken(match('=')?TOKEN_EQUAL_EQUAL:TOKEN_EQUAL);
				break;
			case '<':
				addToken(match('=')?TOKEN_LESS_EQUAL:TOKEN_LESS);
				break;
			case '>':
				addToken(match('=')?TOKEN_GREATER_EQUAL:TOKEN_GREATER);
				break;
			case '"':{
					 string();
					 break;
				 }
			case '(':{
					 addToken(TOKEN_LEFT_PARENTHESE);
					 break;
				 }
			case ')':{
					 addToken(TOKEN_RIGHT_PARENTHESE);
					 break;
				 }
			case '{':{
					 addToken(TOKEN_LEFT_BRACE);
					 break;
				 }
			case '}':{
					 addToken(TOKEN_RIGHT_BRACE);
					 break;
				 }
		default:
			fprintf(stderr,"scan error at line %d,undefined char %c\n",scanner.line,c);
			exit(64);
		};
	}
	addToken(TOKEN_EOF);
	return true;
}

void printScanner(){
	for(int i=0;;i++){
		Token* token = getTokenByIndex(i);
		if (token->type==TOKEN_EOF){
			printf("EOF\n");
			return;
		}
		char* buff=(char*)allocMemory(token->length+1);
		memcpy(buff,token->start,token->length);
		buff[token->length] ='\0';
		printf("TokenType:%d,literal:%s\n",token->type,buff);
	}
}
