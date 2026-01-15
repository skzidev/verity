#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "../../src/lexer/lexer.h"

bool lexer_shouldLog = false;

#define RUN_TEST(source, expected) \
	test_lexer(source, expected, sizeof(expected)/sizeof(expected[0]))

void test_lexer(char* src, TokenKind expected[], int len){
	// Define your unit testing logic here

	printf("running test for source: `%s`\n", src);

	TokenArray tarr = lexer_tokenize(src, "src.vty");

	printf("asserting len: '%d' == '%ld'\n", len, tarr.count);
	assert(tarr.count == (size_t)len && "returned count != expected count");

	for(size_t i = 0; i < tarr.count; i ++){
		TokenKind expectedType = expected[i];
		Token token = tarr.data[i];
		printf("asserting '%d' (%s) == '%d'\n", token.kind, token.lexeme, expectedType);
		assert(token.kind == expectedType && "returned incorrect token kind");
	}
}

int main(){
	// Define arguments for your test here
	TokenKind set1[] = { TOK_MUT, TOK_IDENT, TOK_IDENT, TOK_ASSIGN, TOK_INT, TOK_SEMI, TOK_EOF };
	char* source1 = "mut int x = 0;";

	TokenKind set2[] = { TOK_IMPORT, TOK_STRING, TOK_AS, TOK_IDENT, TOK_SEMI, TOK_EOF };
	char* source2 = "import \"std/fs\" as fs;";

	TokenKind set3[] = { TOK_EXTERNAL, TOK_IDENT, TOK_IDENT, TOK_SEMI, TOK_EOF };
	char* source3 = "external int x;";

	// Run the test here
	RUN_TEST(source1, set1);
	RUN_TEST(source2, set2);
	RUN_TEST(source3, set3);
	printf("[PASS]: Test 'lexer' Passed\n");
	return 0;
}
