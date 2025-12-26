#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "lexer.h"

#include "../diags/diagnostics.h"

void TokenArray_push(TokenArray* array, Token tok){
	if(array->count == array->capacity){
		array->capacity = array->capacity ? array->capacity * 2 : 16;
		array->data = realloc(array->data, array->capacity * sizeof(Token));
	}
	// printf("token: %s @ %d:%d\n", tok.lexeme, tok.line + 1, tok.column);
	array->data[array->count++] = tok;
}

TokenKind lookup(const char* keyword){
	for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
		if (strcmp(keyword, keywords[i].word) == 0) {
			return keywords[i].kind;
		}
	}
	if(isdigit(*keyword) && strchr(keyword, '.') != NULL)
		return TOK_FLOAT;
	else if(isdigit(*keyword))
		return TOK_INT;
	else
		return TOK_IDENT;
}

TokenArray lexer_tokenize(char* input, char* fname){
	int line = 0;
	int column = 0;

	TokenArray tarr = {0};

	int i = 0;
	while(input[i] != '\0'){
		char currentChar = input[i];
		column ++;

		Token token;
		token.lexeme = &currentChar;
		token.column = column;
		token.line = line;

		switch(currentChar){
			case '\n':
				column = 0;
				line ++;
				i ++;
				continue;
			break;
			case '(':
				break;
			case ')':
				token.kind = TOK_RPAREN;
				break;
			case '{':
				token.kind = TOK_LBRACE;
				break;
			case '}':
				token.kind = TOK_RBRACE;
				break;
			case ';':
				token.kind = TOK_SEMI;
				break;
			case ',':
				token.kind = TOK_COMMA;
				break;
			case '\t':
			case ' ':
				i ++;
				continue;
			case '+':
				token.kind = TOK_PLUS;
				break;
			case '/':
				token.kind = TOK_SLASH;
				break;
			case '*':
				token.kind = TOK_STAR;
				break;
			case '-':
				token.kind = TOK_MINUS;
				break;
			case '.':
				token.kind = TOK_PERIOD;
				break;
			case '=':
				token.kind = TOK_ASSIGN;
				break;
			default:
				int start = i;
				while(isalnum(input[i])){
					i ++;
					column ++;
				}
				int len = i - start;
				char *buf = malloc(len + 1);
				memcpy(buf, &input[start], len);
				buf[len] = '\0';
				token.lexeme = buf;

				token.kind = lookup(buf);
				TokenArray_push(&tarr, token);
				continue;
		}
		TokenArray_push(&tarr, token);
		i ++;
	}

	return tarr;
}