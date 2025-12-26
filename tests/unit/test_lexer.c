#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include "../../src/lexer/lexer.h"

void test_lexer(const char* src, TokenKind expected[]){
	// Define your unit testing logic here

	TokenArray tarr = lexer_tokenize(src, "src.vty");

	for(size_t i = 0; i < tarr.count; i ++){
		TokenKind expectedType = expected[i];
		Token token = tarr.data[i];
		assert(token.kind == expectedType);
	}
}

int main(){
	// Define arguments for your test here
	TokenKind expectedTypes[] = { TOK_MUT, TOK_IDENT, TOK_IDENT, TOK_ASSIGN, TOK_INT, TOK_SEMI };
	const char* source = "mut int x = 0;";

	// Run the test here
	test_lexer(source, expectedTypes);
	printf("[PASS]: Test 'lexer' Passed\n");
	return 0;
}