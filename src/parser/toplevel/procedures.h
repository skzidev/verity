#pragma once
#include <stdbool.h>
#include "../structures/identifierlist.h"
#include "../structures/parameterlist.h"
#include "../structures/block.h"

typedef struct {
    bool canRecurse;
    bool isPublic;
    char* ident;
    ParameterList params;
    char* returnType;
    IdentifierList exceptions;
    Block* block;
} ProcedureDefinition;

ProcedureDefinition parser_ProcedureDefinition();
