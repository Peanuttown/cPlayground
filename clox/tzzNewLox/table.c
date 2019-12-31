#include "table.h"
#include "common.h"
#include "memory.h"
#include "string.h"
#include <stdio.h>

#define TABLE_MAX_LOAD 0.85

//alocate enties and init it
static Entry* allocateEntries(int num){
	Entry* entries = ALLOCATE(Entry,num);
	for (int i=0;i < num;i++){
		Entry* entry = &entries[i];
		entry->key = NULL;
		entry->value = NIL_VAL;
	}
	return entries;
}

void initTable(Table* table){
	table->count =0;
	table->cap = 0;
	table->entries = NULL;
}

void freeTable(Table* table){
	freeMemory(table->entries);
	initTable(table);
}

//NULL NULL(TONE) KEY1 KEY2 TONE K3
static Entry* findEntry(Entry* entries,ObjString* key,int cap){
	int index = key->hash % cap;
	Entry* tone  =NULL;
	for(;;){
		Entry* alternate = &entries[index];

		if (alternate->key ==NULL){
			if (IS_NIL(alternate->value)){
				return tone?tone:alternate;
			}else{
				if (tone==NULL) tone = alternate;
			}
		}else if (alternate->key == key){
			return alternate;
		}
		index = (index+1) % cap;
	}
}

static void adjustTable(Table* table,int newCap){
	//allocate a new list entry
	Entry* newEntries = allocateEntries(newCap);

	//record the valid entry count
	int count = 0;
	//move the old entry to new;
	for (int i = 0;i < table->cap; i ++){
		Entry* oldEntry =&table->entries[i];
		//if has value ,we move it
		if (oldEntry->key!=NULL){
			Entry* newEntry=findEntry(newEntries,oldEntry->key,newCap);
			newEntry->key = oldEntry->key;
			newEntry->value = oldEntry->value;
			count++;
		}
	}

	//free the old entries;
	freeMemory(table->entries);
	table->entries =  newEntries;
	table->count = count;
	table->cap = newCap;
}

bool tableSet(Table* table,ObjString* key,Value value){
	if (table->count + 1  > table->count * TABLE_MAX_LOAD){
		int newCap =ARRAY_CAP_GROW(table->cap);
		adjustTable(table,newCap);
	}
	Entry* entry = findEntry(table->entries,key,table->cap);
	bool isNewKey = entry->key ==NULL;
	if (isNewKey && IS_NIL(entry->value)) table->count++;

	entry->key = key;
	entry->value = value;
	return isNewKey;
}

bool tableGet(Table* table,ObjString* key,Value* value){
	Entry* entry = findEntry(table->entries,key,table->cap);
	if (entry->key ==NULL){
		return false;
	}
	*value = entry->value;
	return true;
}

//if we del directly ,we will lose the open address key
//so ,when we del a key,we replace a tone to the entry to means the key may be at the open address whice after the tone
void tableDel(Table* table,ObjString* key){
	Entry* entry = findEntry(table->entries,key,table->cap);
	if (entry->key == NULL){
		return;
	}
	entry->key = NULL;
	entry->value = BOOL_VAL(false);//make a tone and not need to sub count
	return;
}


void tableAddAll(Table* from,Table* to){
	for (int i = 0; i < from->cap; i++){
		Entry* entry = &from->entries[i];
		if (entry->key != NULL){
			tableSet(to,entry->key,entry->value);
		}
	}
}

ObjString* tableFindString(Table* table,char* chars,int length,int hash){
	if (table->count == 0)  {
		return NULL;
	}
	int index =  hash % table->cap;

	for (;;){
		Entry* entry = &table->entries[index];
		if(entry->key ==NULL){
			if (IS_NIL(entry->value)) return NULL;
		}else if(entry->key->length == length && memcmp(entry->key->chars,chars,length)==0){
			return entry->key;
		}
		index =(index+1) % table->cap;
	}
}
