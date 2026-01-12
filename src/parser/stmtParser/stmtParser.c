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

BreakStatement* parser_break_statement(){
    BreakStatement* stmt = malloc(sizeof(BreakStatement));
    parser_expect(TOK_BREAK);
    return stmt;
}

typedef struct {} SkipStatement;

SkipStatement* parser_skip_statement(){
    SkipStatement* stmt = malloc(sizeof(SkipStatement));
    parser_expect(TOK_SKIP);
    return stmt;
}

typedef struct {
    Expression* expr;
} ReturnStatement;

ReturnStatement* parser_return_statement(){
    ReturnStatement* stmt = malloc(sizeof(ReturnStatement));
    parser_expect(TOK_RETURN);
    stmt->expr = parser_expression();
    return stmt;
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
    EQUALS,  // ==
    NOT_EQUALS, // !=
    LESSTHAN, // <
    LESSTHANEQUALTO, // <=
    MORETHAN, // >
    MORETHANEQUALTO // >=
} ConditionOperator;

typedef struct {
    Expression* lhs;
    Expression* rhs;
    ConditionOperator operator;
} Condition;

Condition* parser_condition(){
    Condition* cond = malloc(sizeof(Condition));
    cond->lhs = parser_expression();
    if(tok.kind == TOK_ASSIGN && parser_peek().kind == TOK_ASSIGN){
        cond->operator = EQUALS;
    } else if(tok.kind == TOK_EXCL && parser_peek_for(0).kind == TOK_ASSIGN){
        //                            ^~~~~~~~~~~~~~~ WEIRD BUG FIX DO NOT TOUCH
        parser_advance();
        cond->operator = NOT_EQUALS;
    } else {
        THROW_FROM_USER_CODE(ERROR, filename, tok.line, tok.column, "P0005", "Unknown operator '%s'.", tok.lexeme);
    }
    parser_advance();
    cond->rhs = parser_expression();
    return cond;
}

typedef struct {
    Condition* cond;
    Block* body;
} IfStatement;

IfStatement* parser_if_statement(){
    IfStatement* stmt = malloc(sizeof(IfStatement));
    parser_expect(TOK_IF);
    parser_expect(TOK_LPAREN);
    stmt->cond = parser_condition();
    parser_expect(TOK_RPAREN);
    stmt->body = parser_block();
    return stmt;
}

struct ProcedureCall {
    char* ident;
    ExpressionList* params;
};

ProcedureCall* parser_procedure_call(){
    ProcedureCall* stmt = malloc(sizeof(ProcedureCall));
    stmt->ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_LPAREN);
    stmt->params = parser_expression_list();
    parser_expect(TOK_RPAREN);
    return stmt;
}

typedef enum {
    RETURN,
    BREAK,
    SKIP,
    VARIABLE_DEFINITION,
    VARIABLE_ASSIGNMENT,
    IF,
    ELSEIF,
    ELSE,
    PROCEDURE_CALL
} StatementKind;

struct Statement {
    StatementKind kind;
    union {
        ReturnStatement* retStmt;
        BreakStatement* breakStmt;
        SkipStatement* skipStmt;
        VariableDefinition varDefStmt;
        Assignment varAsgnStmt;
        IfStatement* ifStmt;
        ProcedureCall* call;
    };
};

const char* stmt_kind_to_str(StatementKind stmtkind){
    switch(stmtkind){
        case RETURN:
            return "RETURN";
        case BREAK:
            return "BREAK";
        case SKIP:
            return "SKIP";
        case VARIABLE_DEFINITION:
            return "VAR DEFINE";
        case VARIABLE_ASSIGNMENT:
            return "VAR ASSIGN";
        case IF:
            return "IF";
        case PROCEDURE_CALL:
            return "CALL";
        default:
            return "UNKNOWN";
    }
}

Statement* parser_statement(){
    bool shouldExpectSemi = true;
    // TODO convert to regex or some other more concrete matching
    Statement* stmt = malloc(sizeof(Statement));
    stmt->kind = 0;
    if(tok.kind == TOK_RETURN){
        // return stmt
        stmt->kind = RETURN;
        stmt->retStmt = parser_return_statement();
    } else if(tok.kind == TOK_BREAK) {
        // break stmt
        stmt->kind = BREAK;
        stmt->breakStmt = parser_break_statement();
    } else if(tok.kind == TOK_SKIP){
        // skip stmt
        stmt->kind = SKIP;
        stmt->skipStmt = parser_skip_statement();
    } else if(tok.kind == TOK_MUT
        || (parser_peek_for(1).kind == TOK_ASSIGN
            && parser_peek_for(0).kind == TOK_IDENT
            && tok.kind == TOK_IDENT)){
        // variable definition
        stmt->kind = VARIABLE_DEFINITION;
        stmt->varDefStmt = parser_variable_definition();
    } else if(tok.kind == TOK_IDENT && parser_peek_for(0).kind == TOK_ASSIGN){
        // variable assignment
        stmt->kind = VARIABLE_ASSIGNMENT;
        stmt->varAsgnStmt = parser_assignment();
    } else if(tok.kind == TOK_IF){
        // if statement
        stmt->kind = IF;
        shouldExpectSemi = false;
        stmt->ifStmt = parser_if_statement();
    } else if(tok.kind == TOK_IDENT && parser_peek_for(0).kind == TOK_LPAREN){
        // another weird bug fix
        // procedure call
        stmt->kind = PROCEDURE_CALL;
        stmt->call = parser_procedure_call();
    }
    if(parser_shouldLog)
        printf("\t\tSTATEMENT: kind=%s\n", stmt_kind_to_str(stmt->kind));
    // TODO implement other types of statements
    if(shouldExpectSemi)
        parser_expect(TOK_SEMI);
    return stmt;
}

void StatementList_push(StatementList* stmts, Statement* newItem){
    if(stmts->count == stmts->capacity){
        int new_capacity = (stmts->capacity == 0) ? 4 : stmts->capacity * 2;

        Statement *new_data = realloc(
            stmts->data,
            new_capacity * sizeof(Statement)
        );

        if(!new_data){
            THROW(
                ERROR,
                "P0008",
                "(internal) Memory allocation failed."
            );
            exit(1);
        }

        stmts->data = new_data;
        stmts->capacity = new_capacity;
    }
    stmts->data[stmts->count] = *newItem;
    stmts->count ++;
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

void TopLevelStatementList_push(TopLevelStatementList* stmts, TopLevelStatement* newItem){
    if(stmts->count == stmts->capacity){
        int new_capacity = (stmts->capacity == 0) ? 4 : stmts->capacity * 2;

        TopLevelStatement *new_data = realloc(
            stmts->data,
            new_capacity * sizeof(TopLevelStatement)
        );

        if(!new_data){
            THROW(
                ERROR,
                "P0009",
                "(internal) Memory allocation failed."
            );
            exit(1);
        }

        stmts->data = new_data;
        stmts->capacity = new_capacity;
    }
    stmts->data[stmts->count] = *newItem;
    stmts->count ++;
}

TopLevelStatement* parser_top_level_stmt(){
    TopLevelStatement* stmt = malloc(sizeof(TopLevelStatement));
    stmt->kind = 0;
	if(tok.kind == TOK_IMPORT){
	    stmt->kind = IMPORT;
		stmt->imptStmt = parser_import_statement();
		if(parser_shouldLog)
		    printf("\tIMPORT: mod=%s,ident=%s\n", stmt->imptStmt.package, stmt->imptStmt.ident);
	} else if(tok.kind == TOK_RECURSIVE || tok.kind == TOK_PROC){
	    stmt->kind = PROCEDURE_DEFINITION;
		if(parser_shouldLog)
		    printf("\tPROCEDURE: ident=%s\n", parser_peek_for(0).lexeme);
		stmt->procDef = parser_procedure_definition();
	} else if(tok.kind == TOK_EXTERNAL) {
	    stmt->kind = EXTERN_DECLARATION;
		if(parser_shouldLog)
		    printf("EXTERNAL: \n");
		stmt->externStmt = parser_external_declaration();
	} else {
		THROW_FROM_USER_CODE(ERROR, filename, tok.line, tok.column, "P0002", "unexpected token in top-level statement; did not exepect '%s'", tok.lexeme);
	}
	return stmt;
}