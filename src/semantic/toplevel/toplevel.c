#include "toplevel.h"
#include <stdio.h>

typedef enum {
    PROCEDURE_SYMBOL,
    VARIABLE_SYMBOL
} SymbolKind;

typedef struct {
    int definitionLine;
    char* name;
    SymbolKind k;
} Symbol;

PrimaryExpression semantics_PrimaryExpression(PrimaryExpression expr){
    switch(expr.kind){
        case STRING_LITERAL:
            printf("\t\t\t\tSTRING LITERAL: \"%s\"\n", expr.StringLiteral);
            break;
        case INTEGER_LITERAL:
            printf("\t\t\t\tINT LITERAL: \"%d\"\n", expr.IntegerLiteral);
            break;
        case FLOAT_LITERAL:
            printf("\t\t\t\tFLOAT LITERAL: \"%f\"\n", expr.FloatLiteral);
            break;
        case VARIABLE:
            printf("\t\t\t\tVARIABLE: \"%s\"\n", expr.VariableName);
            break;
        case BOOLEAN_LITERAL:
            printf("\t\t\t\tBOOL LITERAL: \"%b\"\n", expr.BooleanLiteral);
            break;
        case NULL_LITERAL:
            printf("\t\t\t\tNULL\n");
            break;
        case PROCEDURE_CALL_RETURN:
            printf("\t\t\t\tPROC CALL: to \"%s\" with %d parameters \n", expr.call->ident, expr.call->params.count);
            break;
        case SUBEXPRESSION:
            printf("\t\t\t\tSUBEXPR: \n");
            break;
        default: break;
    }
    return expr;
}

ProcedureCall semantics_ProcCall(ProcedureCall stmt);

UnaryExpression semantics_UnaryExpression(UnaryExpression expr){
    semantics_PrimaryExpression(expr.lhs);
    return expr;
}

MulExpression semantics_MulExpression(MulExpression expr){
    semantics_UnaryExpression(expr.lhs);
    if(expr.hasRhs)
        semantics_UnaryExpression(expr.rhs);
    return expr;
}

AddExpression semantics_AddExpression(AddExpression expr){
    semantics_MulExpression(expr.lhs);
    if(expr.hasRhs)
        semantics_MulExpression(expr.rhs);
    return expr;
}

Expression semantics_Expression(Expression expr){
    printf("\t\t\tEXPRESSION:\n");
    semantics_AddExpression(expr.lhs);
    return expr;
}

ReturnStatement semantics_Return(ReturnStatement stmt){
    semantics_Expression(stmt.value);
    return stmt;
}

ProcedureCall semantics_ProcCall(ProcedureCall stmt){
    for(int i = 0; i < stmt.params.count; i ++){
        semantics_Expression(stmt.params.data[i]);
    }
    return stmt;
}

ProcedureDefinition semantics_ProcDef(ProcedureDefinition tp){
    for(int i = 0; i < tp.block->count; i ++){
        switch(tp.block->data[i].kind){
            case RETURN:
                printf("\t\tRETURN\n");
                semantics_Return(tp.block->data[i].returnStatement);
            break;
            case PROCEDURE_CALL:
                printf("\t\tPROC CALL: %s\n", tp.block->data[i].procCall.ident);
                semantics_ProcCall(tp.block->data[i].procCall);
            break;
            case SKIP:
                printf("\t\tSKIP:\n");
            break;
            case VARIABLE_ASGN:
                printf("\t\tVARIABLE ASSIGN: %s\n", tp.block->data[i].varAssignStatement.ident);
            break;
            case VARIABLE_DEF:
                printf("\t\tVARIABLE DEF: %s\n", tp.block->data[i].varDefineStatement.ident);
                semantics_Expression(tp.block->data[i].varDefineStatement.value);
            break;
            case BREAK:
                printf("\t\tBREAK:\n");
            break;
            case IF:
            case ELSEIF:
            case ELSE:
                printf("\t\tCONDITIONAL\n");
            break;
        }
    }
    return tp;
}
