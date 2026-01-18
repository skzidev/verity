#pragma once
#include "./unary.h"

typedef enum {
    MUL,
    DIV
} MulOperator;
typedef struct {
    UnaryExpression lhs;
    UnaryExpression rhs;
    MulOperator op;
    bool hasRhs;
} MulExpression;

MulExpression parser_MulExpression();
