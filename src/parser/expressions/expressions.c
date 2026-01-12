#include "../parser.h"
#include "condition.h"
#include "add.h"
#include "base.h"
#include "mul.h"
#include "primary.h"
#include "unary.h"
#include "../../diags/diagnostics.h"

Expression parser_expression(){
    Expression expr = {0};
    if(tok.kind == TOK_PROPAGATE){
        parser_expect(TOK_PROPAGATE);
        expr.shouldPropogate = true;
    }
    expr.lhs = parser_AddExpression();
    return expr;
}

AddExpression parser_AddExpression(){
    AddExpression expr = {0};
    expr.lhs = parser_MulExpression();
    if(tok.kind == TOK_PLUS || tok.kind == TOK_MINUS){
        if(tok.kind == TOK_PLUS){
            expr.op = ADD;
            parser_expect(TOK_PLUS);
        } else if(tok.kind == TOK_MINUS) {
            expr.op = SUB;
            parser_expect(TOK_MINUS);
        }
        expr.rhs = parser_MulExpression();
    }
    return expr;
}

MulExpression parser_MulExpression(){
    MulExpression expr = {0};
    expr.lhs = parser_UnaryExpression();
    if(tok.kind == TOK_STAR || tok.kind == TOK_SLASH){
        if(tok.kind == TOK_STAR){
            expr.op = MUL;
            parser_expect(TOK_STAR);
        } else if(tok.kind == TOK_SLASH){
            expr.op = DIV;
            parser_expect(TOK_SLASH);
        }
        expr.rhs = parser_UnaryExpression();
    }
    return expr;
}

UnaryExpression parser_UnaryExpression(){
    UnaryExpression expr = {0};
    if(tok.kind == TOK_EXCL){
        expr.op = NOT;
        parser_expect(TOK_EXCL);
    } else if(tok.kind == TOK_MINUS){
        expr.op = NEGATIVE;
        parser_expect(TOK_MINUS);
    }
    expr.lhs = parser_PrimaryExpression();
    return expr;
}

PrimaryExpression parser_PrimaryExpression(){
    PrimaryExpression expr = {0};
    // TODO add subexpressions to this node parser
    switch(tok.kind){
        case TOK_INT:
            expr.kind = INTEGER_LITERAL;
            break;
        case TOK_FLOAT:
            expr.kind = FLOAT_LITERAL;
            break;
        case TOK_STRING:
            expr.kind = STRING_LITERAL;
            break;
        case TOK_IDENT:
            expr.kind = VARIABLE;
            break;
        case TOK_BOOL:
            expr.kind = BOOLEAN_LITERAL;
            break;
        case TOK_NULL:
            expr.kind = NULL_LITERAL;
            break;
        default:
            THROW_FROM_USER_CODE(
                ERROR,
                filename,
                tok.line,
                tok.column,
                "P0002",
                "unexpected token in expression '%s'",
                tok.lexeme
            );
            break;
    }

    return expr;
}

LogicalOperator LogOperator(){
    if(tok.kind == TOK_EXCL && parser_peek(0).kind == TOK_ASSIGN){
        return notEqualTo;
    } else if(tok.kind == TOK_ASSIGN && parser_peek(0).kind == TOK_ASSIGN){
        return equalTo;
    } else if(tok.kind == TOK_LARR){
        if(parser_peek(0).kind == TOK_ASSIGN)
            return lessEqual;
        return lessThan;
    } else if(tok.kind == TOK_RARR){
        if(parser_peek(0).kind == TOK_ASSIGN)
            return greaterEqual;
        return greaterThan;
    } else THROW_FROM_USER_CODE(
        ERROR,
        filename,
        tok.line,
        tok.column,
        "P0006",
        "unknown logical operator '%c'",
        tok.lexeme);
    exit(1);
}

Condition parser_Condition(){
    Condition cond = {0};
    cond.lhs = parser_expression();
    cond.op = LogOperator(tok);
    cond.rhs = parser_expression();
    return cond;
}
