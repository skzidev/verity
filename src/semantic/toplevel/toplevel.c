#include "toplevel.h"
#include "../../diags/diagnostics.h"
#include <stdio.h>
#include "../symbols/symbols.h"

ScopeStack stack = {0};

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
            Symbol* sym = lookup_symbol(&stack, expr.VariableName);
            if(sym == NULL){
                // TODO this 0,0 thing will be confusing. fix.
                // we have to embed document positional data inside the AST structures
                // right now it isn't accessible to us here
                THROW_FROM_USER_CODE(ERROR, filename, 0, 0, "S0001", "use of undefined variable '%s'");
                exit(1);
            }
            // TODO context lookup to find required type
            printf("\t\t\t\tVARIABLE: \"%s\"\n", expr.VariableName);
            break;
        case BOOLEAN_LITERAL:
            printf("\t\t\t\tBOOL LITERAL: \"%b\"\n", expr.BooleanLiteral);
            break;
        case NULL_LITERAL:
            printf("\t\t\t\tNULL\n");
            break;
        case PROCEDURE_CALL_RETURN:
            printf("\t\t\t\tPROC CALL: to \"%s\" with %d parameter(s) \n", expr.call->ident, expr.call->params.count);
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
    if(expr.hasRhs && expr.op == ADD)
        printf("\t\t\t\tOPERATOR: +\n");
    else if(expr.op == SUB)
        printf("\t\t\t\tOPERATOR: -\n");
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
    ScopeStack_push(&stack);
    for(int i = 0; i < tp.block->count; i ++){
        Statement stmt = tp.block->data[i];
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
                Symbol* assignedToSym = lookup_symbol(&stack, stmt.varAssignStatement.ident);
                if(assignedToSym == NULL)
                    THROW_FROM_USER_CODE(ERROR, filename, stmt.line, 0, "S0002", "cannot assign to undeclared variable '%s'", stmt.varAssignStatement.ident);
            break;
            case VARIABLE_DEF:
                printf("\t\tVARIABLE DEF: %s\n", tp.block->data[i].varDefineStatement.ident);
                // Insert this variable into the scope
                Symbol newSym = {0};
                newSym.ident = tp.block->data[i].varDefineStatement.ident;
                newSym.kind = VariableSymbol;
                newSym.next = NULL;
                newSym.definedLine = tp.block->data[i].line;
                newSym.type = tp.block->data[i].varDefineStatement.type;
                ScopeStack_InsertSymbolAtLatestScope(&stack, newSym);
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
    ScopeStack_pop(&stack);
    return tp;
}
