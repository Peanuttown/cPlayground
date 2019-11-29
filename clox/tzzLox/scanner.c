#include "scanner.h"
#include "common.h"
#include "stdlib.h"
#include "string.h"


static char peek(){
	return *scanner.current;
}

static bool isEnd(){
	return *scanner.current == '\0';
}


void initScanner(char* source){
	scanner.source = source;
	scanner.start = scanner.source;
	scanner.current = scanner.source;
}
int slice(){
	return scanner.current - scanner.start;
}

static char advance(){
	scanner.current++;
	return scanner.current[-1];
}

Token makeToken(TokenType type){
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.length = slice();
	return token;
}

static bool check(char expect){
	return  peek() == expect;
}

static char peekNext(){
	if (isEnd()){
		return '\0';
	}
	return scanner.current[1];

}

static bool match(char expect){
	if (check(expect)){
		advance();
		return true;
	}
	return false;
}


static void skipWhitespace(){
	for(;;){
	char c = peek();
		switch(c){
			case '\n':
			case '\t':
			case ' ':
			case '\r':
				advance();
				continue;
			case '/':
				if (peekNext() =='/'){
					scanner.current +=2;
					for(;!isEnd()&&peek()!='\n';){
						advance();
					}
					break;
				}else{
					break;
				}
			default:
				return;
		}
	}
}


static bool isDigit(char c){
	return c<='9' && c >= '0';
}

static bool isAlpha(char c){
	return  (c>='a' && c<='z') || (c >= 'A' && c<= 'Z');
}

static bool isAlphaNumeric(char c){
	return isAlpha(c) || isDigit(c);
}

TokenType checkKeyWord(char* keyword,TokenType type){
	int kwlen = strlen(keyword);//keyword length 
	if (((int)(scanner.current-scanner.start) == kwlen) && memcmp(scanner.start,keyword,kwlen-1)==0){
		return type;
	}
	return TOKEN_IDENTIFIER;
}

TokenType identiferType(){
	char c = *scanner.start;
	switch(c){
		case 'a'://and
			return checkKeyWord("and",TOKEN_AND);
		case 'c'://class
			return checkKeyWord("class",TOKEN_AND);
		case 'e'://else
			return checkKeyWord("else",TOKEN_ELSE);
		case 'f'://false,for,fun
			switch(scanner.start[1]){
				case 'a'://false
					return checkKeyWord("false",TOKEN_FALSE);
				case 'o'://for
					return checkKeyWord("for",TOKEN_FOR);
				case 'u'://fun
					return checkKeyWord("fun",TOKEN_FUN);
			}
		case 'i'://if
					return checkKeyWord("if",TOKEN_IF);
		case 'n'://nil
					return checkKeyWord("nil",TOKEN_NIL);
		case 'o'://or
					return checkKeyWord("or",TOKEN_OR); 
		case 'p'://print
					return checkKeyWord("print",TOKEN_PRINT); 
		case 'r'://return
					return checkKeyWord("return",TOKEN_RETURN); 
		case 's'://super
					return checkKeyWord("super",TOKEN_SUPER); 
		case 't'://this,true
					switch(scanner.start[1]){
						case 'h'://this
							return checkKeyWord("this",TOKEN_THIS);
						case 'r'://true
							return checkKeyWord("true",TOKEN_TRUE);
					}
		case 'v'://var
					return checkKeyWord("var",TOKEN_VAR);
		case 'w'://while
					return checkKeyWord("while",TOKEN_WHILE);
	}
		return TOKEN_IDENTIFIER;
}

Token identifer(){
	for(;!isEnd()&&isAlphaNumeric(peek());){
		advance();
	}
	return makeToken(identiferType());
}

static Token number(){
	for (;!isEnd()&&isDigit(peek());){
		advance();
	}
	if (match('.')){
		for(;!isEnd()&&isDigit(peek());){
			advance();
		}
	}
	return makeToken(TOKEN_NUMBER);
}

static Token string(){
	for(;!isEnd()&&peek()!='"';){
		advance();
	}
	if (isEnd()){
		fprintf(stderr,"scanner.c string() error, unterminated string\n");
		exit(64);
	}
	advance();//consume " 
	Token token =makeToken(TOKEN_STRING);
	token.start++;
	token.length -=2;
	return token;
}

Token scanToken(){
	skipWhitespace();
	scanner.start = scanner.current;
	if (isEnd()){
		return makeToken(TOKEN_EOF);
	}
	char c = advance();
	if (isDigit(c)){
		return number();
	}

	if (isAlpha(c)){
		return identifer();
	}
	switch (c){
		case '=':return makeToken(match('=')?TOKEN_EQUAL_EQUAL:TOKEN_EQUAL);
		case '<':return makeToken(match('=')?TOKEN_LESS_EQUAL:TOKEN_LESS);
		case '>':return makeToken(match('=')?TOKEN_GREATER_EQUAL:TOKEN_GREATER);
		case '!':return makeToken(match('=')?TOKEN_BANG_EQUAL:TOKEN_BANG);
		case '+':return makeToken(TOKEN_PLUS); 
		case '-':return makeToken(TOKEN_MINUS); 
		case '*':return makeToken(TOKEN_STAR); 
		case '/':return makeToken(TOKEN_SLASH); 
		case '.':return makeToken(TOKEN_DOT); 
		case ',':return makeToken(TOKEN_COMMA); 
		case ';':return makeToken(TOKEN_SEMICOLON); 
		case '(':return makeToken(TOKEN_LEFT_PAREN); 
		case ')':return makeToken(TOKEN_RIGHT_PAREN); 
		case '{':return makeToken(TOKEN_LEFT_BRACE); 
		case '}':return makeToken(TOKEN_RIGHT_BRACE); 
		case '"':return string();
		default:
			 fprintf(stderr,"unexpect character\n");
			 exit(64);
	}
}





