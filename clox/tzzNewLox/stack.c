#include "stack.h"
#include <stdio.h>

static bool isFull(Stack *stack){
	return stack->top >= stack->max;
}

static bool isEmpty(Stack *stack){
	return stack->top ==0;
}

void initStack(Stack* stack){
	stack->max = STACK_MAX_SIZE;
	stack->top =0;
}


void push(Stack* stack,Value *value){
	if (isFull(stack)){
		fprintf(stderr,"stack overflow\n");
		exit(64);
	} 
	stack->Values[stack->top] = *value;
	stack->top++;
}

Value* pop(Stack* stack){
	if (isEmpty(stack)){
		return NULL;
	}
	stack->top--;
	return &stack->Values[stack->top];
}
