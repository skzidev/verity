#pragma once

typedef struct Scope {};

typedef struct ScopeStack {
    int count;
    Scope* data;
};

void ScopeStack_push(){}

void ScopeStack_pop(){}

typedef enum {
    VariableSymbol,
    ProcedureSymbol,
    ExternalSymbol
} SymbolKind;

typedef struct {
    SymbolKind kind;
    char* ident;
} Symbol;
