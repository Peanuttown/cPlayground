#include "table.h"
#include "value.h"
#include "memory.h"
#include "stringObject.h"
#include <stdio.h>

void initTable(Table* table){
	table->count = 0;
	table->capacity = 0;
	table->entry= NULL;
}

static Entry* findEntry(Table* table,ObjString* key,int capacity){
	int index = key->hash % capacity ;
	if (index < 0){
		index =-index;
	}
	Entry* entry;
	Entry* stampTone = NULL;
	for (;;){
		entry = &table->entry[index];
		if (entry->key ==NULL)  {
				if (IS_BOOL(entry->value)){//find a stampTone
					stampTone = entry;
					continue;
				}else{
					return stampTone==NULL?entry:stampTone;
				}
		}else if (entry->key==key){
			return entry;
		}
		index =(index+1) % capacity;
	}
}

static void adjustTableCapacity(Table* table,int capacity){
	Entry* entrys = allocMemory(sizeof(Entry)*capacity);

	table->count=0;
	for(int i=0;i<capacity;i++){
		entrys[i].key = NULL;
		entrys[i].value = AS_NIL;
	}

	for (int i=0;i<table->capacity;i++){
		Entry entry= table->entry[i];
		if (entry.key==NULL){
			continue;
		}
		//find new postion
		Entry *dest = findEntry(table,entry.key,capacity);
		dest->key =entry.key;
		dest->value = entry.value;
		table->count++;
	}
	//freeMemory(table->entry);
	table->entry = entrys;
	table->capacity=capacity;
}

bool TableSet(Table* table,ObjString* key,Value* value){
	if ((table->count+1) > table->capacity*TABLE_CAPACITY_MAX_LOAD){
		int capacity = TABLE_GROW_CAPACITY(table->capacity);
		adjustTableCapacity(table,capacity);
	}
	Entry* entry = findEntry(table,key,table->capacity);
	
	bool isNewKey = false;
	if ((entry->key==NULL)&&(!IS_BOOL(entry->value))){
		isNewKey = true;
	}
	if (isNewKey){
		table->count++;
	}
	entry->key = key;
	entry->value= *value;
	return isNewKey;
}

bool TableGet(Table* table,ObjString* key,Value* value){
	Entry* entry = findEntry(table,key,table->capacity);
	if (entry->key == NULL){
		return false;
	}
	*value = entry->value;
	return true;
}

void TableDel(Table* table,ObjString* key){
	//find key
	Entry *entry =findEntry(table,key,table->capacity);
	if (entry->key ==NULL){
	}else{
		entry->key = NULL;
		entry->value = AS_BOOL(true);
	}
	return;
}
