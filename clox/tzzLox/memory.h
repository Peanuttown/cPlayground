#ifndef lox_memory_h
#define lox_memory_h

#include <stdlib.h>

#define GROW_CAPACITY(capacity)\
	((capacity)<8?8:(capacity)*2)

#endif
