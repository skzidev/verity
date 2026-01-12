#pragma once
#include "./import.h"
#include "./procedures.h"
#include "./external.h"

typedef enum {
    IMPORT,
    PROCEDURE,
    EXTERNAL
} TopLevelStatementKind;
typedef struct {
    TopLevelStatementKind kind;
    union {
        ProcedureDefinition procDef;
        ImportStatement impt;
        ExternalDeclaration extDecl;
    };
} TopLevelStatement;

TopLevelStatement parser_TopLevelStatement();
