#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexer.h"

#include "../diags/diagnostics.h"

void TokenArray_push(TokenArray* array, Token tok){
    if(array->count == array->capacity){
		array->capacity = array->capacity ? array->capacity * 2 : 16;
		array->data = realloc(array->data, array->capacity * sizeof(Token));
	}
    if(lexer_shouldLog)
        printf("token: %s (of kind %d) @ %d:%d\n", tok.lexeme, tok.kind, tok.line + 1, tok.column);

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

	int len = strlen(input);

	int i = 0;
	while(i < len && input[i] != '\0'){
		column ++;
		char currentChar = input[i];

		Token token;
		token.lexeme = &currentChar;
		token.column = column;
		token.line = line;

		switch(currentChar){
			case '\n':
				line ++;
				column = 1;
				i ++;
				continue;
			break;
			case '(':
			    token.kind = TOK_LPAREN;
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
				i ++;
				column += 1;
				continue;
			case ' ':
				i ++;
				continue;
			case '+':
				token.kind = TOK_PLUS;
				break;
			case '/':
				token.kind = TOK_SLASH;
				if(input[i + 1] == '/')
					while(input[i] != '\n'){
						i ++;
						column ++;
					}
				else if(input[i + 1] == '=')
					while(input[i] != '=' && input[i + 1] != '/'){
						i ++;
						column ++;
					}
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
			case '"':
				token.kind = TOK_STRING;
				i ++;
				column ++;
				int strStart = i;
				while(input[i] != '"'){
					i ++;
					column ++;
					if(input[i] == '\n')
	                    THROW_FROM_USER_CODE(ERROR, fname, line, column, "L0001", "String literal must be terminated before a newline");
				}
				int strLen = i - strStart;
				char *buffer = malloc(strLen + 1);
				memcpy(buffer, &input[strStart], strLen);
				buffer[strLen] = '\0';
				token.lexeme = buffer;
				TokenArray_push(&tarr, token);
				// free(buffer);
				// buffer = NULL;
				i ++;
				column ++;
				continue;
			case '>':
				token.kind = TOK_RARR;
				break;
			case '<':
				token.kind = TOK_LARR;
				break;
			case '!':
				token.kind = TOK_EXCL;
				break;
			default:
				if(!isalnum(input[i]))
					THROW_FROM_USER_CODE(ERROR, fname, line, column, "L0002", "Unexpected token");
				int start = i;
				while(isalnum(input[i]) || input[i] == '.'){
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
				// free(buf);
				// buf = NULL;
				continue;
		}
		TokenArray_push(&tarr, token);
		i ++;
	}

	Token eof;
	eof.kind = TOK_EOF;
	eof.lexeme = '\0';
	eof.line = line;
	eof.column = column;
	TokenArray_push(&tarr, eof);

	return tarr;
}
