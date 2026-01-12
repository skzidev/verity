#pragma once

typedef struct {
    char* ident;
    // TODO implement ExpressionList
} ProcedureCall;

ProcedureCall parser_ProcedureCall();
