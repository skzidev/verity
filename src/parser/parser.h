#pragma once
#include "../diags/diagnostics.h"
#include "../lexer/lexer.h"
#include "./stmtParser/stmtParser.h"
#include <stdbool.h>

extern bool parser_shouldLog;

typedef struct {
	size_t pos;
	TokenArray* tokens;
} Parser;

extern char* filename;
extern Parser parser;
extern Token tok;

extern void parser_advance();
extern bool parser_accept(TokenKind s);
extern bool parser_expect(TokenKind s);
extern Token parser_peek(void);
extern Token parser_peek_for(int lookahead);
extern void parser_dump_node(TopLevelStatement* s);

typedef struct ProcedureDefinition ProcedureDefinition;
typedef struct ExternalDeclaration ExternalDeclaration;
typedef struct Block Block;


struct Block {
    StatementList* statements;
};

typedef struct {
    TopLevelStatementList* list;
} Program;
extern Program parser_parse(TokenArray* tarr, char* fname);
extern void parser_dump_ast(Program ast);

ProcedureDefinition* parser_procedure_definition();
ExternalDeclaration* parser_external_declaration();
Block* parser_block();
