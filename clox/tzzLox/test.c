#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


void printStrLength(char* str){
	int len = strlen(str);
	printf("%d\n",len);
}

typedef struct{
	int field;
	int field2;
	union{
		int test1;
		char* test2;
	};
}Test;

int main(){
	char *num = "12 3";
	char** test =(char**)malloc(1);
	*test = num;
	double ret =strtod(num,test);
	printf("%s\n",*test);
	printf("%d\n",(int)ret);
	return 0;
}
