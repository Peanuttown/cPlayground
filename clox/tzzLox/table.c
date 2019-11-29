#include "table.h"
#include "common.h"
#include "value.h"
#include "object.h"
#include "memory.h"

void initTalbe(Table* table){
	table->count =0;
	table->capacity=0;
	table->entry=NULL;
}


static uint32_t hash(ObjString* key){
	//todo
	uint32_t hashValue =2166136261u;
	for (int i=0;i<key->length;i++){
		hashValue ^= key->chars[i];
		hashValue *= 222222u;
	}
	return hashValue;
};

Entry* findEntry(Entry *entry,int capacity,ObjString* key){
	uint32_t hashValue = hash(key);
	int index =  hashValue % capacity;
	Entry* tombstone  =NULL;
	for(;;){
		Entry* retEntry =&entry[index];
		if (retEntry->key ==key){
			return retEntry;
		}
		if (retEntry->key == NULL ){
			if (IS_NIL(retEntry->value)){
				return (tombstone==NULL)?retEntry:tombstone;
			}else{//find a tombstone;
				tombstone = retEntry;
			}
		}
		index = (index+1)%capacity;
	}
}

void adjustCapacity(Table* table){
	int newCapacity = table->capacity*2;
	Entry* entry=(Entry*)malloc(sizeof(Entry)*newCapacity);
	for (int i=0;i<newCapacity;i++){
		entry[i].key =NULL;
		entry[i].value =NIL_VAL;
	}
	for(int i =0;i<table->capacity;i++){//reset pos
		Entry *oldEntry = &table->entry[i];
		if (table->entry[i].key!=NULL){
			Entry* newPos =findEntry(entry,newCapacity,oldEntry->key);
			newPos->key = oldEntry->key;
			newPos->value= oldEntry->value;
		}
	}
	table->capacity = newCapacity;
	table->entry = entry;
}

void tableSet(Table *table,ObjString* key,Value value){
	if (table->capacity < table->count + 1){//need grow capacity
		//todo
		//
	}
	uint32_t hashValue = hash(key);
	uint32_t index =  hashValue % table->capacity;

	Entry* entry= findEntry(table->entry,table->capacity,key);
	if (entry->key ==NULL){
		table->count++;
		entry->key = key;
	}
	entry->value = value;
}

bool tableGet(Table* table,ObjString* key,Value* value){
	if (table->count ==0){
		return false;
	}
	Entry* entry = findEntry(table->entry,table->capacity,key);
	if (entry->key==NULL){
		return false;
	}
	*value = entry->value;
	return false;
}
void tableDel(Table* table,ObjString* key){
	if (table->count==0){
		return;
	}
	Entry* entry =findEntry(table->entry,table->capacity,key);
	entry->key = NULL;
	entry->value = BOOL_VAL(true);
	table->count --;
}


