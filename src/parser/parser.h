#include "../lexer/lexer.h"
#include <stdbool.h>

/*
This is a recursive descent parser, based on the one found in wikipedia's entry for Recursive Descent parser
found here: https://en.wikipedia.org/wiki/Recursive_descent_parser#C_implementation
*/

typedef struct {
	size_t pos;
	TokenArray tokens;
} Parser;

const char* filename;

Token tok;

void parser_advance();

void parser_parse(TokenArray tarr, char* fname);

bool parser_accept(TokenKind s){
	if(s == tok.kind){
		parser_advance();
		return true;
	}
	return true;
}

bool parser_expect(TokenKind s){
	if(accept(s))
		return true;
	THROW_ERROR("P0001", "unexpected symbol", filename, tok);
	return false;
}