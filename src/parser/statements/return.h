#pragma once
#include "../expressions/expression.h"

typedef struct {
    Expression value;
} ReturnStatement;

ReturnStatement parser_ReturnStatement();
