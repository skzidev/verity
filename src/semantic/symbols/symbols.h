#pragma once
#include "../../parser/structures/identifierlist.h"
#include <stdbool.h>

typedef enum {
    VariableSymbol,
    ProcedureSymbol,
    ExternalSymbol
} SymbolKind;

typedef struct {
    IdentifierList paramTypes;
    char* returnType;
} ProcSymbol;

typedef struct {
    bool isMutable;
    char* type;
} VarSymbol;

typedef struct Symbol {
    SymbolKind kind;
    char* ident;
    struct Symbol* next;
    int definedLine;
    union {
        VarSymbol varSymbol;
        ProcSymbol procSymbol;
    };
} Symbol;

typedef enum {
    PROCEDURE_DEFINITION,
    LOOP,
    CONDITIONAL
} ScopeType;

typedef struct Scope {
    ScopeType kind;
    Symbol* symbolTable;
    struct Scope* parent;
} Scope;

/**
 * Insert a symbol into a scope
 */
void Scope_insertSymbol(Scope *scope, Symbol sym);

typedef struct ScopeStack {
    Scope* top;
} ScopeStack;

/**
 * Add a new scope onto a ScopeStack
 */
void ScopeStack_push(ScopeStack *stack);

/**
 * Remove the last scope from a ScopeStack
 */
void ScopeStack_pop(ScopeStack* stack);

/**
 * Insert a symbol on the top stack
 */
void ScopeStack_InsertSymbolAtLatestScope(ScopeStack *stack, Symbol sym);

Symbol* lookup_symbol(ScopeStack* stack, const char *name);
