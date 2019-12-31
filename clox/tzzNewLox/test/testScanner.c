#include "../ioutil.h"
#include "../scanner.h"
#include "../token.h"
#include <stdio.h>

void printToken(Token t){
	printf("Token[name:%.*s,type:%d,line:%d]\n",t.length,t.name,t.type,t.line);
}

int main(){
	char* source = readFile("testScanner.txt");
	Scanner scanner;
	scannerInit(&scanner,source);
	for(;;){
		Token t = scanToken(&scanner);
		printToken(t);
		if (t.type ==TOKEN_EOF){
			return 0;
		}
	}
}
