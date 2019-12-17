#include "scanner.h"
#include "common.h"
#include "string.h"

Scanner scanner;

void initScanner(char* source){
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}

static bool isEnd(){
	return *scanner.current == '\0';
}

static Token makeToken(TokenType type){
	Token token;
	token.line = scanner.line;
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current-scanner.start);
	return token;
}

static Token errToken(char* msg){
	Token token;
	token.line = scanner.line;
	token.type = TOKEN_ERROR;
	token.start = msg;
	token.length= (int)strlen(msg);
	return token;
}

char advance(){
	return *scanner.current++;
}

static bool match(char expect){
	if (isEnd()) return false;
	if (*scanner.current == expect){
		advance();
		return true;
	}
	return false;
}

char peek(){
	return *scanner.current;
}

static char peekNext(){
	if (isEnd()) return '\0';
	return scanner.current[1];
}

static void skipWhitespace() {
	for (;;) {                  
		char c = peek();          
		switch (c) {              
			case ' ':               
			case '\r':              
			case '\t':              
				advance();            
				break;
			case '\n':{
					  scanner.line++;
					  scanner.current++;
					  break;
				  }
			case '/':{
					 if (peekNext() == '/'){
						 while(peek()!='\n' && !isEnd()) advance();
					 }else{// not comment
						 return;
					 }
					 break;
				 }

			default:                
				 return;               
		}                         
	}                           
}

static Token string(){
	while(!isEnd() && peek() != '"'){
		scanner.current++;
	}
	if (isEnd()){
		return errToken("unterminted string");
	}
	scanner.current++;
	return makeToken(TOKEN_STRING);
}

static bool isDigit(char c){
	return c >='0' && c<='9';
}


static Token number(){
	while(!isEnd()&&isDigit(peek())){
		advance();
	}
	if (peek()=='.' && isDigit(peekNext())){
		advance();
		while(!isEnd()&&isDigit(peek())){
			advance();
		}
	}
	return makeToken(TOKEN_NUMBER);
}


static bool isAlpha(char c){
	return (c>='a' && c<='z') || (c >= 'A' && c <= 'Z') || c =='_';
}

static TokenType checkKeyword(int start,int length,char* rest,TokenType type){
	if(scanner.current - scanner.start == start + length && memcmp(scanner.start+start,rest,length)==0){
		return type;
	}
	return TOKEN_IDENTIFIER;
}


static TokenType identifierType(){
	switch (scanner.start[0]) {
		case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);      
		case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);  
		case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);    
		case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);        
		case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);      
		case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);        
		case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);  
		case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
		case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);  
		case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);      
		case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);  
		case 'f':                                                     
			  if (scanner.current - scanner.start > 1) {                  
				  switch (scanner.start[1]) {                               
					  case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
					  case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);    
					  case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);    
				  }                                                         
			  }                                                           
			  break;
		case 't':                                                   
			  if (scanner.current - scanner.start > 1) {                
				  switch (scanner.start[1]) {                             
					  case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
					  case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
				  }                                                       
			  }                                                         
			  break;
	}
	return TOKEN_IDENTIFIER;
}

static Token identifer(){
	while(isDigit(peek()) || isAlpha(peek())) advance();

	return makeToken(identifierType());
}

Token scanToken(){
	skipWhitespace();
	scanner.start = scanner.current;
	if (isEnd()){
		return makeToken(TOKEN_EOF);
	}
	char c =advance();
	if (isDigit(c)) return number();
	if (isAlpha(c)) return identifer();
	switch (c) {                                    
		case '(': return makeToken(TOKEN_LEFT_PAREN); 
		case ')': return makeToken(TOKEN_RIGHT_PAREN);
		case '{': return makeToken(TOKEN_LEFT_BRACE); 
		case '}': return makeToken(TOKEN_RIGHT_BRACE);
		case ';': return makeToken(TOKEN_SEMICOLON);  
		case ',': return makeToken(TOKEN_COMMA);      
		case '.': return makeToken(TOKEN_DOT);        
		case '-': return makeToken(TOKEN_MINUS);      
		case '+': return makeToken(TOKEN_PLUS);       
		case '/': return makeToken(TOKEN_SLASH);      
		case '*': return makeToken(TOKEN_STAR);       
		case '!':                                                        
			  return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);  
		case '=':                                                        
			  return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
		case '<':                                                        
			  return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);  
		case '>':                                                        
			  return makeToken(match('=') ?                                  
					  TOKEN_GREATER_EQUAL : TOKEN_GREATER);
		case '"':{
				 return string();
			 }
	}

	return errToken("unexpected character");

}
