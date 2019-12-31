#ifndef lox_value_h
#define lox_value_h

#include "common.h"

#define NUMBER_VAL(value) ((Value){VAL_NUMBER,{.number=value}})
#define NIL_VAL ((Value){VAL_NIL,.as={.number=0}})
#define BOOL_VAL(value) ((Value){VAL_BOOL,.as={.boolean=value}})
#define OBJ_VAL(object) ((Value){VAL_OBJ,.as={.obj=(Obj*)object}})

#define AS_NUMBER(value) ((value).as.number)
#define AS_BOOL(value) ((value).as.boolean)
#define AS_NIL(value) (NULL)
#define AS_OBJ(value) ((value).as.obj)
#define AS_FUNCTION(value) ((ObjFunction*)(AS_OBJ(value)))

#define IS_OBJ(value) ((value).type == VAL_OBJ)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define OBJ_TYPE(value) (AS_OBJ((value))->type)


typedef struct sObj Obj;

typedef enum{
	VAL_NUMBER,
	VAL_NIL,
	VAL_BOOL,
	VAL_OBJ,
}ValueType;


typedef struct{
	ValueType type;
	union{
		double number;
		bool boolean;
		Obj* obj;
	}as;
}Value;

typedef struct sObjString ObjString;

void printValue(Value value);

Value valueAdd(Value a, Value b);



#endif
