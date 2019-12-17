#include "chunk.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void repl(){
	char line[1024];
	for(;;){
		printf(">");

		if (!fgets(line,sizeof(line),stdin)){
			printf("\n");
			break;
		}
		interpret(line);
	}
}

char* readFile(char* path){
	FILE* fd = fopen(path,"r");
	if (fd ==NULL){
		fprintf(stderr,"open file %s failed \n",path);
		exit(64);
	}
	fseek(fd,0,SEEK_END);
	size_t fileSize = ftell(fd);
	rewind(fd);
	char* source = (char*)malloc(fileSize+1);
	size_t bytesRead = fread(source,sizeof(char),fileSize,fd);
	if (bytesRead != fileSize){
		fprintf(stderr,"read file error\n");
		exit(64);
	}
	source[bytesRead] = '\0';
	fclose(fd);
	return source;
}

static void runFile(char* path){
	char* source = readFile(path);
	InterpretResult result = interpret(source);
	free(source);

	if(result == INTERPRET_COMPILE_ERR) exit(65);
	if(result == INTERPRET_RUNTIME_ERR) exit(70);
}

int main(int argc,char** argv){
	initVM();
	if (argc == 1){
		repl();//read evalute print loop
	}else{
		runFile(argv[1]);
	}
	freeVM();
	return 0;
}
