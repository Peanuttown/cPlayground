#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void printStrLength(char* str){
	int len = strlen(str);
	printf("%d\n",len);
}

int main(){
	printStrLength("test");
	return 0;
}
