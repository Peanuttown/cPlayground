#include "ioutil.h"
#include <stdio.h>
#include <stdlib.h>

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
