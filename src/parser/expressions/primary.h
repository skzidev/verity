#pragma once
typedef struct Expression Expression;

typedef enum {
    STRING_LITERAL,
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    BOOLEAN_LITERAL,
    NULL_LITERAL,
    VARIABLE,
    SUBEXPRESSION
} PrimaryKind;
typedef struct {
    PrimaryKind kind;
    union {
        char* StringLiteral;
        int IntegerLiteral;
        float FloatLiteral;
        bool BooleanLiteral;
        int NullLiteral;
        Expression* Subexpression;
    };
} PrimaryExpression;

PrimaryExpression parser_PrimaryExpression();
