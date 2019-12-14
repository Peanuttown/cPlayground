//parse the expression whose precedeing is larget than arg precedence
//so at the top if we pass a smallest precedentce it hope to be parse whole expression
//when a expression is first to be parse ,the prefix is may be:
void parsePrecedence(int precedence){
	//we need the previous and current 
	//group | identifer | unary
	parsePrecedence(previous.precendence)

	while(current.precendence > precedence){//eg ** 
		advance()
		parsePrecedence(previous.precedence)
	}
}
