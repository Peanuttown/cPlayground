#include "vm.h"
int main(int argc,char** argv){
	if (argc==1){
		//read default file
		return runFile("test.lox");
	}else{
		return runFile(argv[1]);
	}

}
