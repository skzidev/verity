#pragma once
#include "../lexer/lexer.h"
#include "expressions/expression.h"

typedef struct {
    TokenArray input;
    size_t pos;
} Parser;

void parser_advance();

extern char* filename;
extern Token tok;
extern Parser parser;

void parser_advance();
Token parser_peek(int lookahead);
void parser_expect(TokenKind s);

void parser_Parse(TokenArray stream, char* fname);
