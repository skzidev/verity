#pragma once
#include "../structures/expressionlist.h"

typedef struct ProcedureCall {
    char* ident;
    ExpressionList params;
} ProcedureCall;

ProcedureCall parser_ProcedureCall();
