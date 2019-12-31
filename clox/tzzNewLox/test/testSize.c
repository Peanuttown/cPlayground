#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct{
	//uint32_t uint32Field;
	bool boolField;
	uint8_t uint8Field;
}S;

int main(){
	S s;
	printf("%lu\n",sizeof(s));
	return 0;
}
