#ifndef lox_token_h
#define lox_token_h



typedef  enum{
	TOKEN_MINUS,//0
	TOKEN_PLUS,//1
	TOKEN_STAR,//2
	TOKEN_SLASH,//3
	TOKEN_DOT,//4
	TOKEN_SEMICOLON,
	TOKEN_EQUAL,
	TOKEN_EQUAL_EQUAL,
	TOKEN_LESS,
	TOKEN_LESS_EQUAL,
	TOKEN_GREATER,
	TOKEN_GREATER_EQUAL,
	TOKEN_EOF,
	TOKEN_NUMBER,
	TOKEN_PRINT,
	TOKEN_VAR,
	TOKEN_IF,
	TOKEN_FOR,
	TOKEN_IDENTIFIER,
	TOKEN_STRING,
}TokenType;


typedef struct{
	char* start;
	int length;
	int line;
	TokenType type;
}Token;


#endif
