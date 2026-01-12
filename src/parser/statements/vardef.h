#pragma once
#include "../expressions/expression.h"
#include <stdbool.h>

typedef struct {
    char* ident;
    char* type;
    Expression value;
    bool isMutable;
} VariableDefinitionStatement;

VariableDefinitionStatement parser_VariableDefinitionStatement();
