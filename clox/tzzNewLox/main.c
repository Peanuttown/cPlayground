#include <stdio.h>
#include "ioutil.h"
#include "vm.h"


int main(){
	char* source = readFile("test.lox");
	return interpret(source);
}


