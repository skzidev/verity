#pragma once
#include "primary.h"

typedef enum {
    NOT,
    NEGATIVE
    // TODO potentially ADDROF and DEREFERENCE
} UnaryOperator;
typedef struct {
    PrimaryExpression lhs;
    UnaryOperator op;
} UnaryExpression;

UnaryExpression parser_UnaryExpression();
