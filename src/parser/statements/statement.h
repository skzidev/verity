#pragma once
#include "break.h"
#include "conditional.h"
#include "procedureCall.h"
#include "skip.h"
#include "varasgn.h"
#include "vardef.h"
#include "return.h"

typedef enum {
    RETURN,
    BREAK,
    SKIP,
    VARIABLE_DEF,
    VARIABLE_ASGN,
    IF,
    ELSEIF,
    ELSE,
    PROCEDURE_CALL
} StatementKind;

typedef struct {
    StatementKind kind;
    int line;
    int start;
    int end;
    union {
        BreakStatement breakStatement;
        ReturnStatement returnStatement;
        SkipStatement skipStatement;
        VariableDefinitionStatement varDefineStatement;
        VariableAssignStatement varAssignStatement;
        ProcedureCall procCall;
        IfStatement ifStmt;
        ElseIfStatement elseifStmt;
        ElseStatement elseStmt;
    };
} Statement;

Statement parser_statement();
