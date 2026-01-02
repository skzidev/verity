#include "./parser.h"
#include <stdio.h>
#include "../lexer/lexer.h"
#include <string.h>
#include <stdlib.h>

#pragma region Non-grammar specific

const char* filename;
Parser parser;
Token tok;

void parser_advance(){
	tok = parser.tokens->data[parser.pos ++];
}

bool parser_accept(TokenKind s){
	if(s == tok.kind){
		parser_advance();
		return true;
	}
	return false;
}

bool parser_expect(TokenKind s){
	if(parser_accept(s))
		return true;
	printf("%d:%d\n", tok.line, tok.column);
	char* errorMessage = (char*) malloc((21 + strlen(tok.lexeme)) * sizeof(char));
	sprintf(errorMessage, "unexpected symbol '%d'; expected '%d'", tok.kind, s);
	THROW_ERROR("P0001", errorMessage, filename, tok);
	return false;
}

#pragma endregion
#pragma region Grammar specific

typedef struct {} Program;

typedef struct {
    const char* package;
    const char* ident;
} ImportStatement;

ImportStatement parser_import_statement(){
    ImportStatement stmt;
	parser_expect(TOK_IMPORT);
	stmt.package = tok.lexeme;
	parser_expect(TOK_STRING);
	parser_expect(TOK_AS);
	stmt.ident = tok.lexeme;
	parser_expect(TOK_IDENT);
	parser_expect(TOK_SEMI);
	return stmt;
}

void parser_parameter_list(){
	while(tok.kind == (TOK_IDENT) || tok.kind == TOK_MUT){
		// consume another parameter
		if(tok.kind == (TOK_MUT)){ parser_advance(); } // mut can be used here too
		parser_expect(TOK_IDENT);
		parser_expect(TOK_IDENT);
		if(tok.kind == TOK_RPAREN) break;
		parser_expect(TOK_COMMA);
	}
}

void parser_expression(){
    // TODO implement
}

void parser_break_statement(){
    parser_expect(TOK_BREAK);
}

void parser_skip_statement(){
    parser_expect(TOK_SKIP);
}

void parser_return_statement(){
    parser_expect(TOK_RETURN);
    parser_expression();
}

typedef struct {
    const char* ident;
    const char* type;
    bool mutable;
    // TODO Expression rhs;
} VariableDefinition;

VariableDefinition parser_variable_definition(){
    VariableDefinition stmt;
    stmt.mutable = (tok.kind == TOK_MUT); // Variable is mutable
    stmt.type = tok.lexeme;
    parser_expect(TOK_IDENT); // type
    stmt.ident = tok.lexeme;
    parser_expect(TOK_IDENT); // identifier
    parser_expect(TOK_ASSIGN);
    parser_expression();
    return stmt;
}

void parser_statement(){
    if(tok.kind == TOK_RETURN){
        // return stmt
        parser_return_statement();
    } else if(tok.kind == TOK_BREAK) {
        // break stmt
        parser_break_statement();
    } else if(tok.kind == TOK_SKIP){
        // skip stmt
        parser_skip_statement();
    }
    // TODO implement other types
    parser_expect(TOK_SEMI);
}

void parser_block(){
	parser_expect(TOK_LBRACE);
	while(tok.kind != (TOK_RBRACE)){
	    // TODO implement statement
	}
	parser_expect(TOK_RBRACE);
}

typedef struct {
    char* type;
} ReturnsClause;

ReturnsClause parser_returns_clause(){
    ReturnsClause stmt;
    parser_expect(TOK_RETURNS);
    stmt.type = tok.lexeme;
	parser_expect(TOK_IDENT);
	return stmt;
}

void parser_throws_clause(){
	parser_expect(TOK_THROWS);
	while(tok.kind == (TOK_IDENT)){
		parser_expect(TOK_IDENT);
		parser_expect(TOK_COMMA);
	}
}

typedef struct {
    char* ident;
    bool recursive;
    // TODO ParameterList params;
    ReturnsClause retClause;
    // TODO ThrowsClause exceptions;
    // TODO Block body;
} ProcedureDefinition;

ProcedureDefinition parser_procedure_definition(){
    ProcedureDefinition stmt;
	stmt.recursive = (tok.kind == (TOK_RECURSIVE));
	parser_advance();
	parser_expect(TOK_PROC);
	stmt.ident = tok.lexeme;
	parser_expect(TOK_IDENT);
	parser_expect(TOK_LPAREN);
	if(tok.kind != (TOK_RPAREN)){
		parser_parameter_list();
	}
	parser_expect(TOK_RPAREN);
	stmt.retClause = parser_returns_clause();
	if(tok.kind == TOK_THROWS){
		parser_throws_clause();
	}
	parser_block();
	return stmt;
}

void parser_external_declaration(){
	// skip the "external" keyword
	parser_advance();
	if(tok.kind == TOK_PROC){
		// External procedure declaration
		parser_expect(TOK_PROC);
		parser_expect(TOK_IDENT);
		parser_expect(TOK_LPAREN);
		parser_parameter_list();
		parser_expect(TOK_RPAREN);
		parser_returns_clause();
		parser_expect(TOK_SEMI);
	} else {
		// external variable declaration
		parser_expect(TOK_IDENT);
		// ^ type
		parser_expect(TOK_IDENT);
		// ^ name
		parser_expect(TOK_SEMI);
	}
}

void parser_top_level_stmt(){
	if(tok.kind == TOK_IMPORT){
		parser_import_statement();
	} else if(tok.kind == TOK_RECURSIVE || tok.kind == TOK_PROC){
		parser_procedure_definition();
	} else if(tok.kind == TOK_EXTERNAL) {
		parser_external_declaration();
	}
	// TODO class definitions
	else {
		THROW_ERROR("P0002", "unexpected token in top-level statement", filename, tok);
	}
}

void parser_program(){
    parser_advance();
	while(tok.kind != (TOK_EOF)){ // note: EOF may not have been implemented yet?
		parser_top_level_stmt();
	}
}

#pragma endregion

void parser_parse(TokenArray* tarr, char* fname){
	filename = fname;

	parser.tokens = tarr;
	parser.pos = 0;

	// Start parsing at the root note: the `Program` node
	parser_program();
}
