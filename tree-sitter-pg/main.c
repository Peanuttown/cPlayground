#include <assert.h>
#include "tree_sitter/api.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../clox/tzzNewLox/ioutil.h"


TSLanguage* tree_sitter_json();
TSLanguage* tree_sitter_go();

static void printNode(TSNode node,int indent){
	int childCount = ts_node_named_child_count(node);
	for (int i = 0;i<indent;i++){
		printf(" ");
	}
	printf("%s",ts_node_type(node));
	for (int i=0;i<childCount;i++){
		printNode(ts_node_named_child(node,i),indent+3);
	}
}

int main(){
	TSParser* parser = ts_parser_new();
	ts_parser_set_language(parser,tree_sitter_go());

	char* source = readFile("./test.go");
	TSTree* tree =ts_parser_parse_string(
			parser,
			NULL,
			source,
			strlen(source)
			);

	TSNode root = ts_tree_root_node(tree);
	printNode(root,0);

	return 0;
}
