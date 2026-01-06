#include "../diags/diagnostics.h"
#include "../lexer/lexer.h"
#include <stdbool.h>

/*
This is a recursive descent parser, based on the one found in wikipedia's entry for Recursive Descent parser
found here: https://en.wikipedia.org/wiki/Recursive_descent_parser#C_implementation
*/

typedef struct {
	size_t pos;
	TokenArray* tokens;
} Parser;

extern char* filename;
extern Parser parser;
extern Token tok;

extern void parser_parse(TokenArray* tarr, char* fname);
extern void parser_advance();
extern bool parser_accept(TokenKind s);
extern bool parser_expect(TokenKind s);
extern Token parser_peek(void);
extern Token parser_peek_for(int lookahead);

typedef struct ProcedureDefinition ProcedureDefinition;
typedef struct ExternalDeclaration ExternalDeclaration;
typedef struct Block Block;
typedef struct StatementList StatementList;

ProcedureDefinition* parser_procedure_definition();
ExternalDeclaration* parser_external_declaration();
Block* parser_block();
