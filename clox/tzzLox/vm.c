#include "vm.h"
#include "common.h"
#include "chunk.h"
#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include <stdarg.h>
#include "chunk.h"
#include "debug.h"
#include <errno.h>

VM vm;

Chunk chunk;

static void resetStack(){
	vm.stackTop = vm.stack;
}

static void push(Value value){
	*vm.stackTop = value;
	vm.stackTop++;
}

static Value pop(){
	if (vm.stackTop == vm.stack){
		return 0;
	}
	vm.stackTop--;
	return *vm.stackTop;

}

InterpreterResult run(){
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
	for(;;){
		uint8_t instruction = READ_BYTE();
		switch (instruction){
			case OP_RETURN: {
						printf("OP_RETURN pop:%g\n",pop());
						return INTERPRETER_RESULT_OK; 
					}
			case OP_CONSTANT:{
						 uint8_t constantPos =READ_BYTE(); 
						 Value constant = READ_CONSTANT();
						 push(constant);
					 }
		}
	}
#undef READ_CONSTANT
#undef READ_BYTE
}

void advance(){
	parser.previous = parser.current;
	parser.current = scanToken();
	printf("parser advance ,now current tokentype %d\n",parser.current.type);
}

static TokenType peek(){
	return parser.current.type;
}

static bool match(int num,...){
	va_list valist;
	va_start(valist,num);
	for (int i=0;i<num;i++){
		if (va_arg(valist,TokenType)==peek()){
			advance();
			return true;
		}
	}
	return false;
}

static TokenType peekPrevious(){
	return parser.previous.type;
}


//send byte to vm.code
static void emitByte(uint8_t byte){
	writeChunk(&chunk,byte);
}

static void emitConstant(Value number){
	printf("emitConstant\n");
	emitByte(OP_CONSTANT);
	int pos = addConstant(&chunk,number);
	emitByte(pos);
}

static void primary(){
	switch (peek()){
	case TOKEN_NUMBER:
		emitConstant(10);//todo
		advance();
		break;
	default:
		fprintf(stderr,"vm.c primary() error ,unexpected tokentype %d\n",peek());
		exit(64);
	}
}


static void unary(){
	if (match(2,TOKEN_BANG,TOKEN_MINUS)){
		TokenType operator = peekPrevious();
		OpCode operatorCode;
		switch(operator){
			case TOKEN_MINUS:
				operatorCode = OP_NEGATE;
				break;
			case TOKEN_BANG:
				operatorCode = OP_NEGATE;
				break;
			default:
				fprintf(stderr,"vm.c unary() error, unexpected tokentype %d\n",operator);
				exit(64);
		}
		unary();
		emitByte(operatorCode);
	}else{
		printf("unary in primary\n");
		primary();
	}
}

static void multion(){
	unary();
	if (match(2,TOKEN_STAR,TOKEN_SLASH)){
		printf("multion match\n");
		TokenType operator = peekPrevious();
		unary();
		OpCode operatorCode;
		switch (operator){
			case TOKEN_SLASH:
				operatorCode = OP_DIV;
				break;
			case TOKEN_STAR:
				operatorCode = OP_MUL;
				break;
			default:
				fprintf(stderr,"vm.c:multion() error,unexpectecd tokentype %d\n",operator);
				exit(64);
		}
		emitByte(operatorCode);
	}
}

//addition: mulition ( +|- multion)*
static void addition(){
	printf("addition call\n");
	multion();
	printf("addition first multion out\n");
	if (match(2,TOKEN_PLUS,TOKEN_MINUS)){
		printf("match addition\n");
		TokenType operator = peekPrevious();
		multion();
		OpCode operatorCode ;
		switch(operator){
			case TOKEN_PLUS:
				operatorCode =OP_ADD;
				break;
			case TOKEN_MINUS:
				operatorCode =OP_SUB;
				break;
			default:
				fprintf(stderr,"vm.c:addition() error,unexpected tokentype %d\n",operator);
				exit(64);
		}
		writeChunk(&chunk,operatorCode);
	}
}

static void expression(){
	addition();
}

static bool isEnd(){
	return peek() ==TOKEN_EOF;
}

void parse(){
	advance();
	for (;!isEnd();){
		expression();
	}
}

void compile(char* source){
	initChunk(&chunk);
	initScanner(source);
	parse();
	disassembleChunk(&chunk);
}

void runFile(char* filename){
	FILE* fd =fopen(filename,"r");
	if (fd == NULL){
		fprintf(stderr,"open file %s error :%s\n",filename,strerror(errno));
		exit(64);
	}
	fseek(fd,0,SEEK_END);
	int fileSize= ftell(fd);
	rewind(fd);
	char *source =(char*)(malloc(fileSize*sizeof(char)+1));
	int bytesRead = fread(source,sizeof(char),fileSize,fd);
	if (bytesRead  != fileSize ){
		fprintf(stderr,"read file %s error :bytesRead:%d,expectRead:%d\n",filename,bytesRead,fileSize);
		exit(64);
	}
	source[bytesRead] = '\0';
	compile(source);
}



InterpreterResult interpret(Chunk* chunk){
	vm.chunk = chunk;
	vm.ip = chunk->code;
	resetStack();
	return run();
}


