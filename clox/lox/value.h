#ifndef lox_value_h
#define lox_value_h

typedef double Value;


typedef struct{
	int count;
	int cap;
	Value* values;
}ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array,Value value);
void freeValueArray(ValueArray* array);
void printValue(Value value);

#endif
