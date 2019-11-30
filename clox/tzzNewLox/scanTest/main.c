#include "../scanner.h"
#include "../ioutil.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char** argv){
	if (argc != 2){
		fprintf(stderr,"Usage:scan PATH\n");
		exit(64);
	}
	char* source = readAll(argv[1]);
	if (source ==NULL){
		exit(64);
	}
	initScanner(source);
	if (!Scan()){
		fprintf(stderr,"scan error \n");
		exit(64);
	}
	printScanner();
}
