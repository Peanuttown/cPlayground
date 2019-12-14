#ifndef lox_stack_h
#define lox_stack_h

#include "value.h"

#define STACK_MAX_SIZE 256

typedef struct{
	Value Values[STACK_MAX_SIZE];
	int max;
	int top;
}Stack;

void initStack(Stack* stack);
void push(Stack* stack ,Value *value);
Value* pop(Stack* stack);

#endif
