#ifndef lox_table_h
#define lox_table_h

#include "value.h"
#include "memory.h"
#include "common.h"

typedef struct{
	ObjString* key;
	Value value;
}Entry;

typedef struct{
	int count;
	int capacity;
	Entry* entry;
}Table;

void initTable(Table* talbe);

void tableSet(Table *table,ObjString* key,Value value);




#endif
