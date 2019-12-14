#include "array.h"
#include <stdio.h>

int main(){

	uint16_t value = 0x111;
	void* temp = (void*)(&value);
	uint16_t v2 =0;
	v2 = *((uint8_t*)(temp));
	printf("%d\n",v2);
	/*
	Array ay ;
	initArray(&ay,sizeof(uint16_t));
	int value =0x231;
	writeArray(&ay,&value);
	uint8_t* popValue = (uint8_t*)getArray(&ay,0);
	printf("%d\n",*popValue);
	return 0;
	*/
}
