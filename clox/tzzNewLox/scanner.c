#include "scanner.h"
#include "token.h"
#include <stdlib.h>
#include "common.h"
#include "string.h"
#include <stdio.h>


void scannerInit(Scanner* scanner,char* source){
	scanner->source =source;
	scanner->current = source;
	scanner->start = source;
	scanner->line = 1;
}



static char peek(Scanner* scanner){
	return *scanner->current;
}

static bool isEnd(Scanner* scanner){
	return peek(scanner) == '\0';
}

static char advance(Scanner* scanner){
	return *scanner->current++;
}

static char peekNext(Scanner* scanner){
	if (isEnd(scanner)){
		return '\0';
	}
	return scanner->current[1];
}

static Token makeToken(Scanner* scanner,TokenType type){
	Token token;
	token.type = type;
	token.length = scanner->current-scanner->start;
	token.name = scanner->start;
	token.line = scanner->line;
	return token;
}

static void skipWhitespace(Scanner *scanner){
	for(;;){
		char c = peek(scanner);
		switch(c){
			case ' ':
			case '\t':
			case '\r':
				advance(scanner);
				break;
			case '\n':
				advance(scanner);
				scanner->line++;
				break;
			case '/':
				if (peekNext(scanner)=='/'){
					advance(scanner);
					advance(scanner);
					for (;!isEnd(scanner)&&peek(scanner)!='\n';){
						advance(scanner);
					}
				}else{
					return;
				}
			default:return;
		}
	}
}

static bool isDigit(char c){
	return c >='0' && c<='9';
}

static bool isAlpha(char c){
	return (c >='a' && c <='z') || (c >='A' && c<='Z');
}

static bool isAlphaNumeric(char c){
	return isAlpha(c) || isDigit(c);
}

static Token number(Scanner* scanner){
	for (;!isEnd(scanner)&&isDigit(peek(scanner));){
		advance(scanner);
	}
	return makeToken(scanner,TOKEN_NUMBER);
}

static Token keyword(Scanner* scanner,int offset,char* rest,TokenType type){
	int restLength = strlen(rest);
	int tokenLength = scanner->current - scanner->start;
	if ((tokenLength != offset+restLength) || (memcmp(rest,&scanner->start[offset],restLength)!=0)){
		return makeToken(scanner,TOKEN_IDENTIFIER);
	} 
	return makeToken(scanner,type);
}

static Token identifierType(Scanner *scanner){
	char start = scanner->start[0];
	switch(start){
		case 't': return keyword(scanner,1,"his",TOKEN_THIS);
		case 'p':{
				 return keyword(scanner,1,"rint",TOKEN_PRINT);
			 }
		case 'f':{
				 switch(scanner->start[1]){
					 case 'u':{
							  return keyword(scanner,2,"nc",TOKEN_FUNC);
						  }
					 case 'o':{
							  return keyword(scanner,2,"r",TOKEN_FOR);
						  }
				 }
				 break;
			 }
	}
	return makeToken(scanner,TOKEN_IDENTIFIER);
}

static Token identifier(Scanner* scanner){
	for(;!isEnd(scanner) && isAlphaNumeric(peek(scanner));){
		advance(scanner);
	}
	return identifierType(scanner);
}

static bool match(Scanner *scanner,char c){
	if (peek(scanner) == c){
		advance(scanner);
		return true;
	}
	return false;

}

static Token string(Scanner* scanner){
	for(;!isEnd(scanner) && peek(scanner)!='"';){
		advance(scanner);
	}
	if (isEnd(scanner)){
		fprintf(stderr,"Unterminated string line : %d",scanner->line);
		exit(64);
	}
	advance(scanner);
	return makeToken(scanner,TOKEN_STRING);
}

Token scanToken(Scanner* scanner){
	skipWhitespace(scanner);
	scanner->start = scanner->current;
	if (isEnd(scanner)){
		return makeToken(scanner,TOKEN_EOF);
	}
	//is identify
	char c = advance(scanner);
	if (isDigit(c)){
		return number(scanner);
	}
	if (isAlpha(c)){
		return identifier(scanner);
	}
	switch(c){
		case '+':{
				 return makeToken(scanner,TOKEN_PLUS);
			 }
		case '-':{
				 return makeToken(scanner,TOKEN_MINUS);
			 }
		case '*':{
				 return makeToken(scanner,TOKEN_STAR);
			 }
		case '/':{
				 return makeToken(scanner,TOKEN_SLASH);
			 }
		case '=':{
				 if (match(scanner,'=')){
					 return makeToken(scanner,TOKEN_EQUAL_EQUAL);
				 }
				 return makeToken(scanner,TOKEN_EQUAL);
			 }
		case '<':{
				 if (match(scanner,'=')){
					 return makeToken(scanner,TOKEN_LESS_EQUAL);
				 }
				 return makeToken(scanner,TOKEN_LESS);
			 }
		case '>':{
				 if(match(scanner,'=')){
					 return makeToken(scanner,TOKEN_GREATER_EQUAL);
				 }
				 return makeToken(scanner,TOKEN_GREATER);
			 }
		case '!':{
				 if (match(scanner,'=')){
					 return makeToken(scanner,TOKEN_BANG_EQUAL);
				 }
				 return makeToken(scanner,TOKEN_BANG);
			 }
		case ';':{
				 return makeToken(scanner,TOKEN_SEMICOLON);
			 }
		case '"':{
				 return string(scanner);
			 }
		default:{
				//unreachable
				fprintf(stderr,"Unexpected character %c\n",c);
				exit(64);
			}
	}
}
