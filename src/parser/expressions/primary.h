#pragma once
typedef struct Expression Expression;

typedef struct ProcedureCall ProcedureCall;
typedef enum {
    STRING_LITERAL,
    INTEGER_LITERAL,
    FLOAT_LITERAL,
    BOOLEAN_LITERAL,
    NULL_LITERAL,
    VARIABLE,
    PROCEDURE_CALL_RETURN,
    SUBEXPRESSION
} PrimaryKind;
typedef struct {
    PrimaryKind kind;
    union {
        char* StringLiteral;
        char* VariableName;
        int IntegerLiteral;
        float FloatLiteral;
        bool BooleanLiteral;
        int NullLiteral;
        Expression* Subexpression;
        ProcedureCall* call;
    };
} PrimaryExpression;

PrimaryExpression parser_PrimaryExpression();
