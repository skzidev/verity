#include "./exprParser.h"
#include "../parser.h"
#include <stdlib.h>

typedef enum {
    PRIMARY_STRING,
    PRIMARY_IDENT,
    PRIMARY_SUBEXPR,
    PRIMARY_INT,
    PRIMARY_FLOAT
} PrimaryKind;

struct PrimaryExpression {
    PrimaryKind kind;
    union {
        char* string;
        char* ident;
        Expression* subexpr;
        int integer;
        float floatingPoint;
    };
};

PrimaryExpression* parser_primary_expression(){
    PrimaryExpression* expr = malloc(sizeof(PrimaryExpression));

    switch(tok.kind){
        case TOK_STRING:
            expr->kind = PRIMARY_STRING;
            expr->string = tok.lexeme;
            parser_advance();
        break;
        case TOK_IDENT:
            expr->kind = PRIMARY_IDENT;
            expr->ident = tok.lexeme;
            parser_advance();
        break;
        case TOK_INT:
            expr->kind = PRIMARY_INT;
            expr->integer = strtol(tok.lexeme, NULL, 10);
            parser_advance();
        break;
        case TOK_FLOAT:
            expr->kind = PRIMARY_FLOAT;
            expr->floatingPoint = strtof(tok.lexeme, NULL);
            parser_advance();
        break;
        default:
            expr->kind = PRIMARY_SUBEXPR;
            expr->subexpr = (parser_expression());
            parser_advance();
        break;
    }

    return expr;
}

typedef enum {
    UNARY_NOT,
    UNARY_NEGATIVE
    // TODO AddressOf w/ @
    // TODO Dereference w/ &
} UnaryOperator;

struct UnaryExpression {
    PrimaryExpression* rhs;
    UnaryOperator operator;
};

UnaryExpression* parser_unary_expression(){
    UnaryExpression* expr = malloc(sizeof(UnaryExpression));
    if(tok.kind == TOK_EXCL || tok.kind == TOK_MINUS){
        if(tok.kind == TOK_EXCL)
            expr->operator = UNARY_NOT;
        else if (tok.kind == TOK_MINUS)
            expr->operator = UNARY_NEGATIVE;
        parser_advance();
    }
    expr->rhs = parser_primary_expression();
    return expr;
}

typedef enum {
    MUL,
    DIV
} MultiplicativeOperator;

struct MulExpression {
    UnaryExpression* lhs;
    UnaryExpression* rhs;
    MultiplicativeOperator operator;
};

MulExpression* parser_mul_expression(){
    MulExpression* expr = malloc(sizeof(MulExpression));
    expr->lhs = parser_unary_expression();
    if(tok.kind == TOK_STAR || tok.kind == TOK_SLASH){
        if(tok.kind == TOK_STAR)
            expr->operator = MUL;
        else if(tok.kind == TOK_SLASH)
            expr->operator = DIV;
        else THROW_FROM_USER_CODE(ERROR, filename, tok.line, tok.column, "P0004", "unknown operator '%s'", tok.lexeme);
        parser_advance();
        expr->rhs = parser_unary_expression();
    }
    return expr;
}

struct Expression {
    bool willPropogate;
    MulExpression* lhs;
    MulExpression* rhs;
    union {
        enum {
            ADD, SUB
        } operator;
    };
};

Expression* parser_expression(){
    Expression* stmt = malloc(sizeof(Expression));
    if(tok.kind == TOK_PROPAGATE){
        stmt->willPropogate = true;
        parser_advance();
    } else stmt->willPropogate = false;
    stmt->lhs = parser_mul_expression();
    if(tok.kind == TOK_PLUS || tok.kind == TOK_MINUS){
        if(tok.kind == TOK_PLUS)
            stmt->operator = ADD;
        else if(tok.kind == TOK_MINUS)
            stmt->operator = SUB;
        else THROW_FROM_USER_CODE(ERROR, filename, tok.line, tok.column, "P0004", "unknown operator '%s'", tok.lexeme);
        parser_advance();
        stmt->rhs = parser_mul_expression();
    }
    return stmt;
}

struct ExpressionList {
    Expression* data;
    int count;
    int capacity;
};

void ExpressionList_push(ExpressionList* params, Expression newItem){
    if(params->count == params->capacity){
        size_t newCapacity = params->capacity ? params->capacity * 2 : 16;
        void* newData = realloc(params->data, newCapacity * sizeof(Expression));

        if(!newData){
            THROW(ERROR, "P0006", "Failed to allocate space");
            exit(1);
        }

        params->capacity = newCapacity;
        params->data = newData;
    }

    params->data[params->count++] = newItem;
}

ExpressionList* parser_expression_list(){
    ExpressionList* list = malloc(sizeof(ExpressionList));
    while(tok.kind != TOK_RPAREN){
        ExpressionList_push(list, *parser_expression());
        printf("%s", tok.lexeme);
    }
    return list;
}
