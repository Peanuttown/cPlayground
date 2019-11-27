#include "vm.h"
#include "chunk.h"
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include "errno.h"
#include "compile.h"
#include "vm.h"
extern int errno;

VM vm;

void initVM(){

}
void freeVM(){
	freeChunk(vm.chunk);
}

static char* readAll(FILE* file){
	rewind(file);
	fseek(file,0,SEEK_END);
	long int size = ftell(file);
	char* buff = (char*)malloc(size+1);
	size_t bytesRead = fread(file,sizeof(char),size,buff);
	if (bytesRead != size){
		fprintf(stderr,"Read file error\n");
		free(buff);
		exit(ERR_READ_FILE);
		return NULL;
	}
	buff[bytesRead] = '\0';
	return buff;
}

InterpretResult runFile(char* file){
	FILE* fd = fopen(file,"r");
	if (fd == NULL){
		fprintf(stderr,"Error opening file '%s' :%s\n",file,strerror(errno));
		return INTERPRET_RESULT_COMPILE_ERR;
	}
	char* source = readAll(fd);
	compile(source);
}


