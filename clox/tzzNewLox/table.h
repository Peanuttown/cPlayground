#ifndef lox_table_h
#define lox_table_h

#include "value.h"

typedef struct{
	ObjString* key;
	Value value;
}Entry;

typedef struct{
	int count;
	int cap;
	Entry* entries;
}Table;

void initTable(Table* table);
void freeTable(Table* table);
bool tableSet(Table* table,ObjString* key,Value value);
bool tableGet(Table* table,ObjString* key,Value *value);
void tableDel(Table* table,ObjString* key);
void tableAddAll(Table* from,Table* to);
ObjString* tableFindString(Table* table,char* chars,int length,int hash);


	

#endif
