#include "toplevel.h"
#include <assert.h>
#include <string.h>
#include "../../diags/diagnostics.h"
#include <stdio.h>
#include "../symbols/symbols.h"
#include "../semantic.h"

char* getStringFromType(Type type){
    if(type == NullType) return NULL;
    if(type == IntType)
        return "int";
    else if(type == StringType)
        return "string";
    else if(type == FloatType)
        return "float";
    else if(type == BoolType)
        return "bool";
    else return "(composite type)";
}

Type getTypeFromString(char* str){
    if(str == NULL) return NullType;
    if(strcmp(str, "int") == 0)
        return IntType;
    else if(strcmp(str, "string") == 0)
        return StringType;
    else if(strcmp(str, "float") == 0)
        return FloatType;
    else if(strcmp(str, "bool") == 0)
        return BoolType;
    else return CompositeType;
}

Type semantics_PrimaryExpression(PrimaryExpression expr){
    switch(expr.kind){
        case STRING_LITERAL:
            return StringType;
        case INTEGER_LITERAL:
            return IntType;
        case FLOAT_LITERAL:
            return FloatType;
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
            return getTypeFromString(sym->varSymbol.type);
        case BOOLEAN_LITERAL:
            return BoolType;
        case NULL_LITERAL:
            return NullType;
        case PROCEDURE_CALL_RETURN:
            // printf("\t\t\t\tPROC CALL: to \"%s\" with %d parameter(s)\n", expr.call->ident, expr.call->params.count);
            Symbol* callTo = lookup_symbol(&stack, expr.call->ident);
            if(callTo == NULL){
                THROW_FROM_USER_CODE(ERROR, filename, expr.line, 0, "S0008", "cannot call undefined procedure '%s'", expr.call->ident);
                exit(1);
            }
            if(callTo->kind == VariableSymbol){
                THROW_FROM_USER_CODE(ERROR, filename, expr.line, 0, "S0012", "cannot call non-procedure symbol '%s'", expr.call->ident);
                exit(1);
            }
            Type retType = getTypeFromString(callTo->procSymbol.returnType);
            return retType;
        case SUBEXPRESSION:
            break;
    }
    return NullType;
}

ProcedureCall semantics_ProcCall(ProcedureCall stmt);

Type semantics_UnaryExpression(UnaryExpression expr){
    return semantics_PrimaryExpression(expr.lhs);
}

Type semantics_MulExpression(MulExpression expr){
    Type leftType = semantics_UnaryExpression(expr.lhs);
    Type rightType = {0};
    if(expr.hasRhs)
        rightType = semantics_UnaryExpression(expr.rhs);
    if(expr.hasRhs && leftType != rightType)
        THROW_FROM_USER_CODE(ERROR, filename, 0, 0, "S0009", "operator type mismatch: %s is not compatible for operations with %s", getStringFromType(leftType), getStringFromType(rightType));
    return leftType;
}

Type semantics_AddExpression(AddExpression expr){
    Type leftType = semantics_MulExpression(expr.lhs);
    Type rightType = {0};
    if(expr.hasRhs)
        rightType = semantics_MulExpression(expr.rhs);
    if(expr.hasRhs && leftType != rightType)
        THROW_FROM_USER_CODE(ERROR, filename, 0, 0, "S0009", "operator type mismatch: %s is not compatible for operations with %s", getStringFromType(leftType), getStringFromType(rightType));
    return leftType;
}

Type semantics_Expression(Expression expr){
    return semantics_AddExpression(expr.lhs);
}

ReturnStatement semantics_Return(ReturnStatement stmt){
    semantics_Expression(stmt.value);
    return stmt;
}

IdentifierList semantics_StripTypesFromParameterList(ParameterList* list){
    IdentifierList result = {0};
    for(int i = 0; i < list->count; i++){
        IdentifierList_push(&result, list->data[i].type);
    }
    return result;
}

ProcedureCall semantics_ProcCall(ProcedureCall stmt){
    Symbol* calledTo = lookup_symbol(&stack, stmt.ident);
    if(calledTo == NULL){
        // TODO append more location information to the AST so this error has a actual location
        THROW_FROM_USER_CODE(ERROR, filename, 0, 0, "S0006", "call to undefined procedure '%s'", stmt.ident);
    }
    if(calledTo->kind == VariableSymbol){
        THROW_FROM_USER_CODE(ERROR, filename, 0,0, "S0010", "cannot call non-procedure symbol '%s'", stmt.ident);
        return stmt;
    }
    char** expected = calledTo->procSymbol.paramTypes.data;
    int expectedCount = calledTo->procSymbol.paramTypes.count;
    if(stmt.params.count != expectedCount) {
        THROW_FROM_USER_CODE(ERROR, filename, 0, 0, "S0013", "parameter count mismatch: procedure '%s' expects %d parameters but got %d", stmt.ident, calledTo->procSymbol.paramTypes.count, stmt.params.count);
        return stmt;
    }
    if(expectedCount > 0){
        DiagnosticAssertion(expected != NULL);
        for(int i = 0; i < stmt.params.count; i ++){
            Expression expr = stmt.params.data[i];
            Type exprType = semantics_Expression(expr);
            if(getTypeFromString(expected[i]) != exprType)
                THROW_FROM_USER_CODE(ERROR, filename, 0,0, "S0011", "type mismatch: '%s' is passed as parameter, but the procedure requires a parameter of type '%s'", getStringFromType(exprType), expected[i]);
        }
    }
    return stmt;
}

ProcedureDefinition semantics_ProcDecl(ProcedureDefinition tp){
    Symbol ProcSym = {0};
    ProcSym.ident = tp.ident;
    ProcSym.varSymbol.isMutable = false;
    ProcSym.kind = ProcedureSymbol;
    ProcSym.procSymbol.paramTypes = semantics_StripTypesFromParameterList(&tp.params);
    ProcSym.procSymbol.returnType = tp.returnType;
    ProcSym.definedLine = tp.line;

    Symbol* existingProcDef = lookup_symbol(&stack, tp.ident);
    if(existingProcDef != NULL){
        THROW_FROM_USER_CODE(ERROR, filename, tp.line, 0, "S0005", "cannot redefine procedure '%s' (originally defined %d)", tp.ident, existingProcDef->definedLine);
        exit(1);
    }
    ScopeStack_InsertSymbolAtLatestScope(&stack, ProcSym);
    return tp;
}

ProcedureDefinition semantics_ProcDef(ProcedureDefinition tp){
    ScopeStack_push(&stack);
    for(int i = 0; i < tp.params.count; i ++){
        Parameter param = tp.params.data[i];
        Symbol sym = {0};
        sym.definedLine = tp.line;
        sym.ident = param.ident;
        sym.varSymbol.isMutable = param.isMutable;
        sym.kind = VariableSymbol;
        sym.next = NULL;
        sym.varSymbol.type = param.type;
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
                else if(assignedToSym->varSymbol.isMutable != true){
                    THROW_FROM_USER_CODE(ERROR, filename, stmt.line, 0, "S0003", "cannot assign to immutable value '%s'", stmt.varAssignStatement.ident);
                    exit(1);
                }
            break;
            case VARIABLE_DEF:
                // Insert this variable into the scope
                Symbol* assignedTo = lookup_symbol(&stack, tp.block->data[i].varDefineStatement.ident);
                Symbol newSym = {0};
                newSym.ident = tp.block->data[i].varDefineStatement.ident;
                if(assignedTo != NULL && strcmp(assignedTo->ident, newSym.ident) == 0){
                    THROW_FROM_USER_CODE(ERROR, filename, stmt.line, 0, "S0004", "cannot define reuse variable identifier '%s' within the current scope (first defined on line %d)", newSym.ident, assignedTo->definedLine);
                    exit(1);
                }
                newSym.kind = VariableSymbol;
                newSym.next = NULL;
                newSym.definedLine = tp.block->data[i].line;
                newSym.varSymbol.isMutable = stmt.varDefineStatement.isMutable;
                newSym.varSymbol.type = tp.block->data[i].varDefineStatement.type;
                ScopeStack_InsertSymbolAtLatestScope(&stack, newSym);
                // analyze the expression
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
