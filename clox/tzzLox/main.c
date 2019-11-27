#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc,char** argv){
	if (argc != 2){
		fprintf(stderr,"Usage: lox PATH");
		exit(1);
	}
	runFile(argv[1]);
	return 0;
}
