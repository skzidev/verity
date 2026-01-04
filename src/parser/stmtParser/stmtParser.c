#include "../parser.h"
#include "../exprParser/exprParser.h"
#include <stdlib.h>
#include "stmtParser.h"

struct ImportStatement {
    const char* package;
    const char* ident;
};

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

typedef struct {} BreakStatement;

void parser_break_statement(){
    parser_expect(TOK_BREAK);
}

typedef struct {} SkipStatement;

void parser_skip_statement(){
    parser_expect(TOK_SKIP);
}

typedef struct {
    Expression* expr;
} ReturnStatement;

void parser_return_statement(){
    parser_expect(TOK_RETURN);
    parser_expression();
}

typedef struct {
    char* ident;
    char* type;
    bool mutable;
    Expression* rhs;
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

typedef struct {
    char* ident;
    Expression* rhs;
} Assignment;

Assignment parser_assignment(){
    Assignment stmt;
    stmt.ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_ASSIGN);
    stmt.rhs = parser_expression();
    return stmt;
}

typedef enum {
    RETURN,
    BREAK,
    SKIP,
    VARIABLE_DEFINITION,
    VARIABLE_ASSIGNMENT,
} StatementKind;

struct Statement {
    StatementKind kind;
    union {
        ReturnStatement retStmt;
        BreakStatement breakStmt;
        SkipStatement skipStmt;
        VariableDefinition varDefStmt;
        Assignment varAsgnStmt;
    };
};

Statement* parser_statement(){
    Statement* stmt = malloc(sizeof(Statement));
    if(tok.kind == TOK_RETURN){
        // return stmt
        stmt->kind = RETURN;
        parser_return_statement();
    } else if(tok.kind == TOK_BREAK) {
        // break stmt
        parser_break_statement();
    } else if(tok.kind == TOK_SKIP){
        // skip stmt
        parser_skip_statement();
    } else if(tok.kind == TOK_MUT
        || (parser_peek_for(2).kind == TOK_ASSIGN
            && parser_peek_for(1).kind == TOK_IDENT
            && tok.kind == TOK_IDENT)){
        // variable definition
        parser_variable_definition();
    } else if(tok.kind == TOK_IDENT && parser_peek().kind == TOK_ASSIGN){
        // variable assignment
        parser_assignment();
    }
    // TODO implement other types
    parser_expect(TOK_SEMI);
    return stmt;
}

typedef enum {
    IMPORT,
    PROCEDURE_DEFINITION,
    EXTERN_DECLARATION
} TopLevelStatementType;

struct TopLevelStatement {
    TopLevelStatementType kind;
    union {
        ImportStatement imptStmt;
        ProcedureDefinition* procDef;
        ExternalDeclaration* externStmt;
    };
};

TopLevelStatement* parser_top_level_stmt(){
    TopLevelStatement* stmt = malloc(sizeof(TopLevelStatement));
	if(tok.kind == TOK_IMPORT){
	    stmt->kind = IMPORT;
		stmt->imptStmt = parser_import_statement();
	} else if(tok.kind == TOK_RECURSIVE || tok.kind == TOK_PROC){
	    stmt->kind = PROCEDURE_DEFINITION;
		stmt->procDef = parser_procedure_definition();
	} else if(tok.kind == TOK_EXTERNAL) {
	    stmt->kind = EXTERN_DECLARATION;
		stmt->externStmt = parser_external_declaration();
	}
	// TODO class definitions
	else {
		THROW_FROM_USER_CODE(ERROR, filename, tok.line, tok.column, "P0002", "unexpected token in top-level statement; did not exepect '%s'", tok.lexeme);
	}
	return stmt;
}
