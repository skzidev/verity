#pragma once
#include "../expressions/expression.h"

typedef struct {
    char* ident;
    Expression value;
} VariableAssignStatement;

VariableAssignStatement parser_VariableAssignStatement();
