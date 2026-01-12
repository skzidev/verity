#pragma once
#include "../expressions/expression.h"

typedef struct {
    int count;
    int capacity;
    Expression* data;
} ExpressionList;

void ExpressionList_push(ExpressionList* list, Expression item);
ExpressionList parser_ExpressionList();
