#include "ioutil.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "memory.h"
#include <stdlib.h>


char* readAll(char* file){
	FILE* fd=fopen(file,"r");
	if (fd==NULL){
		fprintf(stderr,"open file error:%s\n",strerror(errno));
		return NULL;
	}
	fseek(fd,0,SEEK_END);
	int fileSize = ftell(fd);
	rewind(fd);
	char* buff = (char*)allocMemory(sizeof(char)*(fileSize+1));
	size_t bytesRead =fread(buff,sizeof(char),fileSize,fd);
	if (bytesRead != fileSize){
		fprintf(stderr,"read file error:fileSize:%d,readByte:%d\n",fileSize,(int)bytesRead);
		return NULL;
	}
	buff[bytesRead] ='\0';
	return buff;
}

