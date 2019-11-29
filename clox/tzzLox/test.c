#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void printStrLength(char* str){
	int len = strlen(str);
	printf("%d\n",len);
}

int main(){
	char* s1 ="tzz";
	char* s2 ="tz";
	if (memcmp(s1,s2,sizeof(char)*2)==0){
		printf("match\n");
	}
	printStrLength("test");
	return 0;
}
