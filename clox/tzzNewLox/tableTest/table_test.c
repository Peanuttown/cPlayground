#include "../table.h"
#include "../value.h"
#include "../stringObject.h"
#include <string.h>
#include <stdio.h>
#include "../common.h"

int main(){
	Table t;
	initTable(&t);
	char* keyChar ="key1";
	char* keyChar2 = "key1";
	ObjString* key1=copyString(keyChar,strlen(keyChar));
	ObjString* key2=copyString(keyChar2,strlen(keyChar2));
	printf("%p,%p\n",key1,key2);
	TableSet(&t,key1,&AS_NUMBER(20));
	Value result;
	if (TableGet(&t,key2,&result)){
		printValue(&result);
		TableSet(&t,key2,&AS_OBJ((Obj*)key1));
		if(TableGet(&t,key2,&result)){
			printValue(&result);
		}
	}else{
		fprintf(stderr,"get failed\n");
		exit(64);
	}
	return 0;
}
