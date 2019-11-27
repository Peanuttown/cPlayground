#ifndef lox_value_h
#define lox_value_h

typedef double Value;

typedef struct{
	int count;
	int capacity;
	Value* values;
}ValueArray;

void initValueArray(ValueArray* array);
int writeValueArray(ValueArray* array,Value value);
void freeValueArray(ValueArray* arry);

#endif
