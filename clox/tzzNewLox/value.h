#ifndef lox_value_h
#define lox_value_h

#include "common.h"

#define AS_NUMBER(value) ((Value){VAL_NUMBER,{.number=value}})
#define AS_NIL ((Value){VAL_NIL})
#define AS_BOOL(value) ((Value){VAL_BOOL,{.boolean=value}})
#define AS_OBJ(object) ((Value){VAL_OBJ,{.obj=object}})

#define IS_BOOL(value) \
	((value.type) == VAL_BOOL)


#define ALLOCATE_OBJ(type,objType) \
	(type*)(allocObj(sizeof(type),objType));

typedef enum{
	VAL_NIL,
	VAL_NUMBER,
	VAL_BOOL,
	VAL_OBJ,
}ValueType;

typedef enum{
	OBJ_STRING,
}ObjType;

typedef struct sObjString ObjString;

typedef struct{
	ObjType type;
}Obj;


typedef struct{
	ValueType type;
	union{
		bool boolean;
		double number;
		Obj* obj;
	};
}Value;


void printValue(Value* value);

Obj* allocObj(size_t size,ObjType type);



#endif

