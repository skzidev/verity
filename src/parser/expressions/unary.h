#pragma once
#include "primary.h"

typedef enum {
    NOT,
    NEGATIVE
    // TODO pointer operators like referenceto and dereference
} UnaryOperator;
typedef struct {
    PrimaryExpression lhs;
    UnaryOperator op;
} UnaryExpression;

UnaryExpression parser_UnaryExpression();
