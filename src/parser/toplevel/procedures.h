#pragma once
#include <stdbool.h>
#include "../structures/identifierlist.h"
#include "../structures/parameterlist.h"

typedef struct {
    bool canRecurse;
    char* ident;
    ParameterList params;
    char* returnType;
    IdentifierList exceptions;
} ProcedureDefinition;

ProcedureDefinition parser_ProcedureDefinition();
