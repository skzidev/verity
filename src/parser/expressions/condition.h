#pragma once
#include "./base.h"

typedef enum {
    equalTo,
    notEqualTo,
    greaterThan,
    greaterEqual,
    lessThan,
    lessEqual
} LogicalOperator;

// TODO logical operator char* lookup table and function

typedef struct {
    Expression lhs;
    LogicalOperator op;
    Expression rhs;
} Condition;

Condition parser_Condition();
