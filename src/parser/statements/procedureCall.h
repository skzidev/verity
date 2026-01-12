#pragma once
#include "../structures/expressionlist.h"

typedef struct {
    char* ident;
    ExpressionList params;
} ProcedureCall;

ProcedureCall parser_ProcedureCall();
