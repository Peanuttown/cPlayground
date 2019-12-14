#ifndef lox_table_h
#define lox_table_h

#include "value.h"
#define TABLE_INIT_SIZE 10
#define  TABLE_CAPACITY_MAX_LOAD 0.75
#define TABLE_GROW_CAPACITY(cap) \
	(((cap)<8?8:(cap)*2))

typedef struct{
	ObjString* key;
	Value value;
}Entry;

typedef struct{
	Entry* entry;//an array of buckets
	int capacity;
	int count;
}Table;

void initTable(Table* table);
void freeTable(Table* table);

bool TableSet(Table* table,ObjString* key,Value* value);

bool TableGet(Table* table,ObjString *key,Value* value);
void TableDel(Table* table,ObjString *key);

#endif
