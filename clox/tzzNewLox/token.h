#ifndef lox_token_h
#define lox_token_h


typedef enum{
	TOKEN_EOF,
	TOKEN_IDENTIFIER,
	TOKEN_THIS,
	TOKEN_FOR,
	TOKEN_FUNC,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_SLASH,
	TOKEN_EQUAL,
	TOKEN_EQUAL_EQUAL,
	TOKEN_BANG,
	TOKEN_BANG_EQUAL,
	TOKEN_LESS,
	TOKEN_LESS_EQUAL,
	TOKEN_GREATER,
	TOKEN_GREATER_EQUAL,
	TOKEN_NUMBER,

	TOKEN_PRINT,
	TOKEN_SEMICOLON,
	TOKEN_STRING,
	TOKEN_VAR,
}TokenType;



typedef struct{
	TokenType type;
	char* name;
	int length;
	int line;
}Token;

#endif
