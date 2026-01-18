#pragma once
#include <stdlib.h>
#include "./mul.h"

typedef enum {
    ADD,
    SUB
} AddOperator;

typedef struct {
    MulExpression lhs;
    MulExpression rhs;
    AddOperator op;
    bool hasRhs;
} AddExpression;

AddExpression parser_AddExpression();
