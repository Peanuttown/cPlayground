#include "value.h"
#include "object.h"
#include <stdio.h>
#include <stdlib.h>

static void printObj(Obj* obj){
	switch(obj->type){
		case (OBJ_STRING):{
			ObjString* str = (ObjString*)obj;
			printf("%.*s",str->length,str->chars);
			break;
		}
		case (OBJ_FUNCTION):{
			//todo
			break;
		}
		default:{
				fprintf(stderr,"printObj error:Unexpect obj type %d\n",obj->type);
				break;
			}
	}
}

void printValue(Value value){
	switch(value.type){
		case(VAL_NUMBER):{
					 printf("%g",AS_NUMBER(value));
					 break;
				 }
		case(VAL_BOOL):{
				       printf("%s",AS_BOOL(value)?"true":"false");
				       break;
				 }
		case(VAL_OBJ):{
				      printObj(AS_OBJ(value));
				      break;
				 }
		case(VAL_NIL):{

				 }
		default:{

			}
	}

}

Value valueAdd(Value a,Value b){
	if (IS_NUMBER(a)&&IS_NUMBER(b)){
		return NUMBER_VAL(AS_NUMBER(a)+AS_NUMBER(b));
	}
	fprintf(stderr,"valueAdd error\n");
	exit(64);
}
