#ifndef clox_value_h
#define clox_value_h

typedef double Value;

typedef struct{
	int count;
	int capacity;
	Value* values;
}ValueArray;

void initValueArray(ValueArray* valueArray);
void freeValueArray(ValueArray* valueArray);
void writeValueArray(ValueArray* valueArray,Value value);

#endif
