#include "scanner.h"
#include "token.h"
#include "common.h"
#include "stdio.h"
#include "errno.h"
#include "stdlib.h"
#include "string.h"

void initScanner(Scanner* scanner,char* source){
	scanner->source = source;
	scanner->start = source;
	scanner->current= source;
	scanner->line = 1;
};

static char advance(Scanner* scanner){
	scanner->current++;
	return scanner->current[-1];
}

static bool isEnd(Scanner* scanner){
	return *scanner->current == '\0';
}


char peek(Scanner* scanner){
	return *scanner->current;
}

static bool match(Scanner* scanner,char expect){
	if (peek(scanner) == expect){
		advance(scanner);
		return true;
	}
	return false;
}

static bool consume(Scanner* scanner,char expect,char* errDesc){
	if (match(scanner,expect)){

		return true;
	}
	fprintf(stderr,"scanner error at line %d %s\n",scanner->line,errDesc);
	exit(ERR_SCAN_UNEXPECTED_CHAR);
}


static bool check(Scanner* scanner,char c){
	return peek(scanner) == c;
}

static char peekNext(Scanner* scanner){
	if (isEnd(scanner)){
		return '\0';
	}
	return scanner->current[1];
}

//todo
void skipWhitespace(Scanner* scanner){
	for (;;){
		char c = peek(scanner);
		switch (c){
			case '\t':
			case '\r':
			case ' ':
				advance(scanner);
				break;
			case '\n':
				scanner->line++;
				advance(scanner);
				break;
			case '/':
				if (peekNext(scanner) == '/'){
					scanner->current++;
					for (;!isEnd(scanner)&& peek(scanner) != '\n';){
						advance(scanner);
					}
				}else{
					return;
				}
				break;
			default:
				return;
		}
	}
}

static bool isDigit(char c){
	return c >= '0' && c <= '9';
}

static bool isAlpha(char c){
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

static bool isAlphaNumeric(char c){
	return isDigit(c) || isAlpha(c);
}

Token makeToken(Scanner* scanner,TokenType type){
	Token token;
	token.type = type;
	token.start = scanner->start;
	token.length = (int)(scanner->current-scanner->start);
	token.line = scanner->line;
	return token;
}

Token number(Scanner* scanner){
	for (;isDigit(peek(scanner));){
		advance(scanner);
	}
	if (match(scanner,'.')){
		for(;isDigit(peek(scanner));){
			advance(scanner);
		}
	}
	return makeToken(scanner,TOKEN_NUMBER);
}

Token string(Scanner* scanner){
	for (;isAlphaNumeric(peek(scanner));){
		advance(scanner);
	}
	consume(scanner,'"',"unterminted string");
	return makeToken(scanner,TOKEN_STRING);
}

TokenType checkWord(Scanner* scanner,char* word,int start,int length,TokenType type){
	if ((scanner->current - scanner->start == start+length)&& memcpy(scanner->start+start,word,length)){
		return type;
	}
	return TOKEN_IDENTIFIER;

}

TokenType identiferType(Scanner* scanner){
	switch (scanner->start[0]){
		case 'a': return checkWord(scanner,"nd",1,2,TOKEN_AND);
		case 'c': return checkWord(scanner,"lass",1,4,TOKEN_AND);
		case 'e': return checkWord(scanner,"lse",1,3,TOKEN_AND);
		case 'f': {
				  if ((scanner->current - scanner->start) > 1){
					  switch (scanner->start[1]){
						  case 'a':return checkWord(scanner,"lse",2,3,TOKEN_FOR);
						  case 'o':return checkWord(scanner,"r",2,1,TOKEN_FOR);
						  case 'u':return checkWord(scanner,"n",2,1,TOKEN_FUN);
					  }
				  }
				  break;
			  }
		case 'i':return checkWord(scanner,"f",1,1,TOKEN_IF);
		case 'n':return checkWord(scanner,"il",1,2,TOKEN_NIL);
		case 'o':return checkWord(scanner,"r",1,1,TOKEN_OR);
		case 'p':return checkWord(scanner,"rint",1,4,TOKEN_PRINT);
		case 'r':return checkWord(scanner,"eturn",1,5,TOKEN_RETURN);
		case 's':return checkWord(scanner,"uper",1,4,TOKEN_SUPER);
		case 't':{
				 if ((scanner->current - scanner->start)>1){
					 switch (scanner->start[1]){
						 case 'h':return checkWord(scanner,"is",2,2,TOKEN_THIS);
						 case 'r':return checkWord(scanner,"ue",2,2,TOKEN_TRUE);
					 }
				 }
				 break;
			 }
		case 'v':return checkWord(scanner,"ar",1,2,TOKEN_VAR);
		case 'w':return checkWord(scanner,"hile",1,4,TOKEN_WHILE);


	}
	return TOKEN_IDENTIFIER;
}

Token identifer(Scanner* scanner){
	for (;isAlphaNumeric(peek(scanner));){
		advance(scanner);
	}
	return makeToken(scanner,identiferType(scanner));
}







Token scanToken(Scanner* scanner){
	skipWhitespace(scanner);
	scanner->start = scanner->current;
	if  (isEnd(scanner)){
		return makeToken(scanner,TOKEN_EOF);

	}
	char c = advance(scanner);
	if (isDigit(c)){
		return number(scanner);
	}

	if (isAlpha(c)){
		return identifer(scanner);
	}
	switch (c){
		case '"': return string(scanner);
		case '(': return makeToken(scanner,TOKEN_LEFT_PAREN); 
		case ')': return makeToken(scanner,TOKEN_RIGHT_PAREN);
		case '{': return makeToken(scanner,TOKEN_LEFT_BRACE); 
		case '}': return makeToken(scanner,TOKEN_RIGHT_BRACE);
		case ';': return makeToken(scanner,TOKEN_SEMICOLON);  
		case ',': return makeToken(scanner,TOKEN_COMMA);      
		case '.': return makeToken(scanner,TOKEN_DOT);        
		case '-': return makeToken(scanner,TOKEN_MINUS);      
		case '+': return makeToken(scanner,TOKEN_PLUS);       
		case '/': return makeToken(scanner,TOKEN_SLASH);      
		case '*': return makeToken(scanner,TOKEN_STAR);
		case '!':                                                        
			  return makeToken(scanner,match(scanner,'=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);  
		case '=':                                                        
			  return makeToken(scanner,match(scanner,'=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
		case '<':                                                        
			  return makeToken(scanner,match(scanner,'=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);  
		case '>':                                                        
			  return makeToken(scanner,match(scanner,'=')? TOKEN_GREATER_EQUAL:TOKEN_GREATER);
		default:
			  fprintf(stderr,"unexpected char %c \n",c);
			  exit(ERR_SCAN_UNEXPECTED_CHAR);
	}
}


