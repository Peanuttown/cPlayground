#include "array.h"
#include <stdio.h>

int main(){
	Array ay ;
	initArray(&ay,sizeof(int));
	int value =8;
	writeArray(&ay,&value);
	printf("%d\n",*(int*)getArray(&ay,0));
	value = 9;
	writeArray(&ay,&value);
	printf("%d\n",*(int*)getArray(&ay,1));
	return 0;
}
