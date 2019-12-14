#include "value.h"
#include <stdio.h>
#include "memory.h"
#include "stringObject.h"



void printValue(Value* value){
	switch (value->type){
		case VAL_NIL:{
				     printf("nil\n");
				     break;
			     }
		case VAL_NUMBER:{
					printf("%f\n",value->number);
					break;
				}
		case VAL_BOOL:{
				      printf("bool:%d\n",value->boolean);
				      break;
			      }
		case VAL_OBJ:{
				     switch(value->obj->type){
					     case OBJ_STRING:{
								     ObjString* s = (ObjString*)(value->obj);
								     printf("objString:%.*s\n",s->length,s->str);
								     break;
							     } 
					     default:{
							     fprintf(stderr,"undefined obj type%d\n",value->obj->type);
							     break;
						     }
				     }
				     break;
			     }
		default:{
				fprintf(stderr,"undefined value type %d",value->type);
				return;
			}
	}
}


Obj* allocObj(size_t size,ObjType type){
	Obj* obj =(Obj*)allocMemory(size);
	obj->type = type;
	return obj;

}
