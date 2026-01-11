#pragma once
#include "../parser.h"
#include "../exprParser/exprParser.h"

typedef struct ImportStatement ImportStatement;
typedef struct TopLevelStatement TopLevelStatement;
typedef struct Statement Statement;
typedef struct ProcedureCall ProcedureCall;
typedef struct StatementList StatementList;
typedef struct TopLevelStatementList TopLevelStatementList;
typedef struct BreakStatement BreakStatement;
typedef struct SkipStatement SkipStatement;
typedef struct ReturnStatement ReturnStatement;
typedef struct VariableDefinition VariableDefinition;
typedef struct Assignment Assignment;
typedef struct Condition Condition;
typedef struct IfStatement IfStatement;
typedef struct ParameterList ParameterList;

typedef struct {
    char* type;
    char* ident;
    bool mutable;
} Parameter;

struct ParameterList {
    Parameter* data;
    int count;
    int capacity;
};

struct StatementList {
    int count;
    int capacity;
    Statement* data;
};

struct TopLevelStatementList {
    int count;
    int capacity;
    TopLevelStatement* data;
};

struct ImportStatement {
    const char* package;
    const char* ident;
};

struct BreakStatement {};

struct SkipStatement {};

struct ReturnStatement {
    Expression* expr;
};

struct VariableDefinition {
    char* ident;
    char* type;
    bool mutable;
    Expression* rhs;
};

struct Assignment {
    char* ident;
    Expression* rhs;
};

typedef enum {
    EQUALS,  // ==
    NOT_EQUALS, // !=
    LESSTHAN, // <
    LESSTHANEQUALTO, // <=
    MORETHAN, // >
    MORETHANEQUALTO // >=
} ConditionOperator;

struct Condition {
    Expression* lhs;
    Expression* rhs;
    ConditionOperator operator;
};

struct ProcedureCall {
    char* ident;
    ExpressionList* params;
};

struct IfStatement {
    Condition* cond;
    Block* body;
};

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

typedef enum {
    EXTERNAL_PROC_DECLARATION,
    EXTERNAL_VARIABLE_DECLARATION
} ExternalDeclarationType;

typedef struct {
    char* ident;
    ParameterList params;
    char* retType;
} ExternalProcedureDeclaration;

typedef struct {
    char* ident;
    char* type;
    Expression* rhs;
    bool mutable;
} ExternalVariableDeclaration;

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

TopLevelStatement* parser_top_level_stmt();

void StatementList_push(StatementList* list, Statement* item);
void TopLevelStatementList_push(TopLevelStatementList* list, TopLevelStatement* item);
ProcedureCall* parser_procedure_call();
Statement* parser_statement();
