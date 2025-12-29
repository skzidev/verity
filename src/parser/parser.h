#include "../lexer/lexer.h"

typedef struct {
	size_t pos;
	TokenArray tokens;
	Program ast;
} Parser;

typedef struct {
	// follow grammar down: define import_statement and procedure_declaration as top_level_statement
} Program;

void parser_parse(TokenArray tarr, char* fname);