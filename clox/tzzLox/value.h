#ifndef lox_value_h
#define lox_value_h

#define BOOL_VAL(value) ((Value){VAL_BOOLEAN,{.boolean=value}})
#define NIL_VAL ((Value){VAL_NIL,{.number=0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER,{.number=value}})
#define OBJ_VAL(value) ((Value){VAL_OBJ,{.obj=(Obj*)(value)}})

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)
#define AS_OBJ(value) ((value).as.obj)

#define IS_BOOL(value) ((value).type == VAL_BOOLEAN)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_OBJ(value) ((value).type==VAL_OBJ)

#include "common.h"

typedef enum{
	VAL_NIL,
	VAL_NUMBER,
	VAL_BOOLEAN,
	VAL_OBJ,
}ValueType;

typedef struct sObj Obj;
typedef struct sObjString ObjString;

typedef struct{
	ValueType type;
	union{
		bool boolean;
		double number;
		Obj* obj;
	}as;
}Value;

typedef struct{
	int count;
	int capacity;
	Value* values;
}ValueArray;

void initValueArray(ValueArray* array);
int writeValueArray(ValueArray* array,Value value);
void freeValueArray(ValueArray* arry);

#endif
