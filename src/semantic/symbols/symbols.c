#include "symbols.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void Scope_insertSymbol(Scope* scope, Symbol sym){
    Symbol* newSymbol = (Symbol*) malloc(sizeof(Symbol));
    newSymbol->ident = sym.ident;
    newSymbol->kind = sym.kind;
    newSymbol->varSymbol.type = sym.varSymbol.type;
    newSymbol->next = sym.next;
    newSymbol->definedLine = sym.definedLine;
    newSymbol->next = scope->symbolTable;
    scope->symbolTable = newSymbol;
}

void ScopeStack_push(ScopeStack* stack){
    Scope* new_scope = (Scope *)malloc(sizeof(Scope));
    new_scope->symbolTable = NULL;
    new_scope->parent = stack->top;
    stack->top = new_scope;
}

void ScopeStack_pop(ScopeStack* stack){
    if (stack->top != NULL) {
        Scope* old_scope = stack->top;
        stack->top = stack->top->parent;
        free(old_scope);
    }
}

void ScopeStack_InsertSymbolAtLatestScope(ScopeStack* stack, Symbol sym){
    if(stack->top != NULL)
        Scope_insertSymbol(stack->top, sym);
}

Symbol* lookup_symbol(ScopeStack* stack, const char *name) {
    Scope *current_scope = stack->top;
    while (current_scope != NULL) {
        Symbol *symbol = current_scope->symbolTable;
        while (symbol != NULL) {
            if(symbol->ident == NULL) continue;
            if (strcmp(symbol->ident, name) == 0) {
                return symbol;
            }
            symbol = symbol->next;
        }
        current_scope = current_scope->parent;
    }
    return NULL;
}
