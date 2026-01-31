#include "toplevel.h"
#include <string.h>
#include "../../diags/diagnostics.h"
#include <stdio.h>
#include "../symbols/symbols.h"
#include "../semantic.h"

PrimaryExpression semantics_PrimaryExpression(PrimaryExpression expr){
    switch(expr.kind){
        case STRING_LITERAL:
            break;
        case INTEGER_LITERAL:
            break;
        case FLOAT_LITERAL:
            break;
        case VARIABLE:
            Symbol* sym = lookup_symbol(&stack, expr.VariableName);
            if(sym == NULL){
                // TODO this 0,0 thing will be confusing. fix.
                // we have to embed document positional data inside the AST structures
                // right now it isn't accessible to us here
                THROW_FROM_USER_CODE(ERROR, filename, 0, 0, "S0001", "use of undefined variable '%s'", expr.VariableName);
                exit(1);
            }
            // TODO context lookup to find required type
            break;
        case BOOLEAN_LITERAL:
            break;
        case NULL_LITERAL:
            break;
        case PROCEDURE_CALL_RETURN:
            // printf("\t\t\t\tPROC CALL: to \"%s\" with %d parameter(s)\n", expr.call->ident, expr.call->params.count);
            break;
        case SUBEXPRESSION:
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
    if(expr.hasRhs && expr.op == ADD);
    else if(expr.op == SUB){}
    if(expr.hasRhs)
        semantics_MulExpression(expr.rhs);
    return expr;
}

Expression semantics_Expression(Expression expr){
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
    Symbol ProcSym = {0};
    ProcSym.ident = tp.ident;
    ProcSym.isMutable = false;
    ProcSym.kind = ProcedureSymbol;
    ProcSym.definedLine = tp.line;
    ScopeStack_InsertSymbolAtLatestScope(&stack, ProcSym);
    ScopeStack_push(&stack);
    for(int i = 0; i < tp.params.count; i ++){
        Parameter param = tp.params.data[i];
        Symbol sym = {0};
        sym.definedLine = tp.line;
        sym.ident = param.ident;
        sym.isMutable = param.isMutable;
        sym.kind = VariableSymbol;
        sym.next = NULL;
        sym.type = param.type;
        ScopeStack_InsertSymbolAtLatestScope(&stack, sym);
    }
    for(int i = 0; i < tp.block->count; i ++){
        Statement stmt = tp.block->data[i];
        switch(tp.block->data[i].kind){
            case RETURN:
                semantics_Return(tp.block->data[i].returnStatement);
            break;
            case PROCEDURE_CALL:
                semantics_ProcCall(tp.block->data[i].procCall);
            break;
            case SKIP:
            break;
            case VARIABLE_ASGN:
                Symbol* assignedToSym = lookup_symbol(&stack, stmt.varAssignStatement.ident);
                if(assignedToSym == NULL){
                    THROW_FROM_USER_CODE(ERROR, filename, stmt.line, 0, "S0002", "cannot assign to undeclared variable '%s'", stmt.varAssignStatement.ident);
                    exit(1);
                }
                else if(assignedToSym->isMutable != true){
                    THROW_FROM_USER_CODE(ERROR, filename, stmt.line, 0, "S0003", "cannot assign to immutable value '%s'", stmt.varAssignStatement.ident);
                    exit(1);
                }
                free(assignedToSym);
            break;
            case VARIABLE_DEF:
                // Insert this variable into the scope
                Symbol* assignedTo = lookup_symbol(&stack, tp.block->data[i].varDefineStatement.ident);
                Symbol newSym = {0};
                newSym.ident = tp.block->data[i].varDefineStatement.ident;
                if(assignedTo != NULL && strcmp(assignedTo->ident, newSym.ident) == 0){
                    THROW_FROM_USER_CODE(ERROR, filename, stmt.line, 0, "S0004", "cannot define variable with variable of same name '%s' (first defined on line %d)", newSym.ident, assignedTo->definedLine);
                    exit(1);
                }
                newSym.kind = VariableSymbol;
                newSym.next = NULL;
                newSym.definedLine = tp.block->data[i].line;
                newSym.isMutable = stmt.varDefineStatement.isMutable;
                newSym.type = tp.block->data[i].varDefineStatement.type;
                ScopeStack_InsertSymbolAtLatestScope(&stack, newSym);
                // analyze the expression
                free(assignedTo);
                semantics_Expression(tp.block->data[i].varDefineStatement.value);
            break;
            case BREAK:
            break;
            case IF:
            case ELSEIF:
            case ELSE:
            break;
        }
    }
    ScopeStack_pop(&stack);
    return tp;
}
