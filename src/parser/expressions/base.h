#pragma once
#include <stdbool.h>
#include "./add.h"

struct Expression {
    bool shouldPropogate;
    AddExpression lhs;
};

Expression parser_expression();
