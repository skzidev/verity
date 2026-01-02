#include "./parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "../diags/diagnostics.h"
#include "../lexer/lexer.h"

#pragma region Non-grammar specific

char* filename;
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
	printf("%s\n", tok.lexeme);
	THROW_FROM_USER_CODE(ERROR,
	    filename,
		tok.line,
		tok.column,
		"P0001",
		"unexpected token '%s'",
		tok.lexeme
	);
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

typedef struct {
    char* type;
    char* ident;
    bool mutable;
} Parameter;

typedef struct {
    Parameter* data;
    int count;
    int capacity;
} ParameterList;

void ParameterList_push(ParameterList* params, Parameter newItem){
    if(params->count == params->capacity){
        size_t newCapacity = params->capacity ? params->capacity * 2 : 16;
        void* newData = realloc(params->data, newCapacity * sizeof(Parameter));

        if(!newData){
            THROW(ERROR, "P0003", "Failed to allocate space");
            exit(1);
        }

        params->capacity = newCapacity;
        params->data = newData;
    }

    params->data[params->count++] = newItem;
}

ParameterList parser_parameter_list(){
    ParameterList stmt = {};
	while(tok.kind == (TOK_IDENT) || tok.kind == TOK_MUT){
		Parameter param = {};
		if(tok.kind == (TOK_MUT)){
		    param.mutable = true;
			parser_advance();
		}
		param.type = tok.lexeme;
		parser_expect(TOK_IDENT);
		param.ident = tok.lexeme;
		parser_expect(TOK_IDENT);
		if(tok.kind == TOK_RPAREN) break;
		parser_expect(TOK_COMMA);
		ParameterList_push(&stmt, param);
	}
	return stmt;
}

typedef struct {

} Expression;

Expression parser_expression(){
    // TODO implement
    if(tok.kind == TOK_PROPAGATE){
        parser_advance();
    }
    Expression stmt = {};
    return stmt;
}

typedef struct {} BreakStatement;

void parser_break_statement(){
    parser_expect(TOK_BREAK);
}

typedef struct {} SkipStatement;

void parser_skip_statement(){
    parser_expect(TOK_SKIP);
}

typedef struct {
    Expression expr;
} ReturnStatement;

void parser_return_statement(){
    parser_expect(TOK_RETURN);
    parser_expression();
}

typedef struct {
    char* ident;
    char* type;
    bool mutable;
    Expression rhs;
} VariableDefinition;

VariableDefinition parser_variable_definition(){
    VariableDefinition stmt = {
        .mutable=false,
    };
    if(tok.kind == TOK_MUT){
        stmt.mutable = true;
        parser_advance();
    }
    stmt.type = tok.lexeme;
    parser_expect(TOK_IDENT); // type
    stmt.ident = tok.lexeme;
    parser_expect(TOK_IDENT); // identifier
    parser_expect(TOK_ASSIGN);
    stmt.rhs = parser_expression();
    return stmt;
}

typedef struct {} Statement;

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
	    parser_statement();
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

typedef struct {
    char* start;
    int count;
    int capacity;
} IdentifierList;

IdentifierList parser_throws_clause(){
    IdentifierList stmt = {
        .capacity=0,
        .count=0
    };
	parser_expect(TOK_THROWS);
	while(tok.kind == (TOK_IDENT)){
		parser_expect(TOK_IDENT);
		parser_expect(TOK_COMMA);
	}
	return stmt;
}

typedef struct {
    char* ident;
    bool recursive;
    ParameterList params;
    ReturnsClause retClause;
    // TODO ThrowsClause exceptions;
    // TODO Block body;
} ProcedureDefinition;

ProcedureDefinition parser_procedure_definition(){
    ProcedureDefinition stmt = {};
    if(tok.kind == (TOK_RECURSIVE)){
        stmt.recursive = true;
        parser_advance();
    }
	parser_expect(TOK_PROC);
	stmt.ident = tok.lexeme;
	parser_expect(TOK_IDENT);
	parser_expect(TOK_LPAREN);
	if(tok.kind != (TOK_RPAREN)){
		stmt.params = parser_parameter_list();
	}
	parser_expect(TOK_RPAREN);
	stmt.retClause = parser_returns_clause();
	if(tok.kind == TOK_THROWS){
		parser_throws_clause();
	}
	parser_block();
	return stmt;
}

typedef enum {
    EXTERNAL_PROC_DECLARATION,
    EXTERNAL_VARIABLE_DECLARATION
} ExternalDeclarationType;

typedef struct {
    char* ident;
    ParameterList params;
    char* retType;
    IdentifierList errors;
} ExternalProcedureDeclaration;

typedef struct {
    char* ident;
    char* type;
    Expression rhs;
    bool mutable;
} ExternalVariableDeclaration;

typedef struct {
    ExternalDeclarationType type;
    union {
        ExternalProcedureDeclaration *procDecl;
        ExternalVariableDeclaration *varDecl;
    };
} ExternalDeclaration;

ExternalDeclaration parser_external_declaration(){
    ExternalDeclaration stmt = {};
	// skip the "external" keyword
	parser_advance();
	if(tok.kind == TOK_PROC){
	    stmt.type = EXTERNAL_PROC_DECLARATION;
		// External procedure declaration
		parser_expect(TOK_PROC);
		parser_expect(TOK_IDENT);
		parser_expect(TOK_LPAREN);
		stmt.procDecl->params = parser_parameter_list();
		parser_expect(TOK_RPAREN);
		parser_returns_clause();
		parser_expect(TOK_SEMI);
	} else {
	    stmt.type = EXTERNAL_VARIABLE_DECLARATION;
		// external variable declaration
		if(tok.kind == TOK_MUT){
		    stmt.varDecl->mutable = true;
			parser_advance();
		}
		stmt.varDecl->type = tok.lexeme;
		parser_expect(TOK_IDENT);
		// ^ type
		stmt.varDecl->ident = tok.lexeme;
		parser_expect(TOK_IDENT);
		// ^ name
		parser_expect(TOK_SEMI);
	}
	return stmt;
}

void parser_top_level_stmt(){
	if(tok.kind == TOK_IMPORT){
		parser_import_statement();
	} else if(tok.kind == TOK_RECURSIVE || tok.kind == TOK_PROC){
		parser_procedure_definition();
	} else if(tok.kind == TOK_EXTERNAL) {
		parser_external_declaration();
	}
	// TODO class definitionss
	else {
		THROW_FROM_USER_CODE(ERROR, filename, tok.line, tok.column, "P0002", "unexpected token in top-level statement; did not exepect '%s'", tok.lexeme);
	}
}

void parser_program(){
    parser_advance();
	while(tok.kind != (TOK_EOF)){
		parser_top_level_stmt();
	}
}

#pragma endregion

void parser_parse(TokenArray* tarr, char* fname){
	filename = fname;

	parser.tokens = tarr;
	parser.pos = 0;

	// Start parsing at the root node: the `Program` node
	parser_program();
}
