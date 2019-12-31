#include "../array.h"
#include <stdio.h>

void printInt(void* elem){
	printf("%d\n",*(int*)(elem));
}

int main(){
	Array array;
	ARRAY_INIT(&array,int);
	int a =10;
	int b = 20;
	int c = 30;
	arrayWrite(&array,&a);
	arrayWrite(&array,&b);
	arrayWrite(&array,&c);
	arrayIterator(&array,printInt);
	return 0;
}
