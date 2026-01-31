#pragma once
#include "../structures/identifierlist.h"

typedef struct {
    char* ident;
    char* type;
} External_VariableDeclaration;

typedef struct {
    char* ident;
    IdentifierList params;
    char* returnType;
} External_ProcedureDeclaration;

typedef enum {
    EXTERNAL_VARIABLEDECL,
    EXTERNAL_PROCDECL
} ExternalKind;
typedef struct {
    ExternalKind kind;
    int line;
    union {
        External_ProcedureDeclaration procDecl;
        External_VariableDeclaration varDecl;
    };
} ExternalDeclaration;

ExternalDeclaration parser_ExternalDeclaration();
