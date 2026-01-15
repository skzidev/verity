#include "./toplevel.h"
#include "../parser.h"
#include "external.h"
#include "import.h"
#include "procedures.h"
#include "../../diags/diagnostics.h"

ProcedureDefinition parser_ProcedureDefinition(){
    ProcedureDefinition stmt = {0};
    if(tok.kind == TOK_PUBLIC){
        stmt.isPublic = true;
        parser_expect(TOK_PUBLIC);
    }
    if(tok.kind == TOK_RECURSIVE){
        stmt.canRecurse = true;
        parser_expect(TOK_RECURSIVE);
    }
    parser_expect(TOK_PROC);
    stmt.ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_LPAREN);
    stmt.params = parser_ParameterList();
    parser_expect(TOK_RETURNS);
    stmt.returnType = tok.lexeme;
    parser_expect(TOK_IDENT);
    stmt.block = parser_Block();
    return stmt;
}

ImportStatement parser_ImportStatement(){
    ImportStatement stmt = {0};
    parser_expect(TOK_IMPORT);
    stmt.package = tok.lexeme;
    parser_expect(TOK_STRING);
    parser_expect(TOK_AS);
    stmt.ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_SEMI);
    return stmt;
}

External_ProcedureDeclaration parser_ExternalProcdef(){
    External_ProcedureDeclaration stmt = {0};
    parser_expect(TOK_PROC);
    stmt.ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_LPAREN);
    stmt.params = parser_IdentifierList();
    parser_expect(TOK_RPAREN);
    parser_expect(TOK_RETURNS);
    stmt.returnType = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_SEMI);
    THROW(NOTE, no_code, "found decl for external proc '%s' which returns '%s'", stmt.ident, stmt.returnType);
    return stmt;
}

External_VariableDeclaration parser_ExternalVariableDecl(){
    External_VariableDeclaration decl = {0};
    decl.type = tok.lexeme;
    parser_expect(TOK_IDENT);
    decl.ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_SEMI);
    THROW(NOTE, no_code, "found decl for external var '%s' of type '%s'", decl.ident, decl.type);
    return decl;
}

ExternalDeclaration parser_ExternalDeclaration(){
    ExternalDeclaration decl = {0};
    parser_expect(TOK_EXTERNAL);
    if(tok.kind == TOK_PROC){
        decl.kind = EXTERNAL_PROCDECL;
        decl.procDecl = parser_ExternalProcdef();
    } else {
        decl.kind = EXTERNAL_VARIABLEDECL;
        decl.varDecl = parser_ExternalVariableDecl();
    }
    return decl;
}

TopLevelStatement parser_TopLevelStatement(){
    TopLevelStatement stmt = {0};
    printf("tok kind: %d\n", tok.kind);
    if(tok.kind == TOK_IMPORT){
        stmt.kind = IMPORT;
        stmt.impt = parser_ImportStatement();
    } else if(tok.kind == TOK_EXTERNAL){
        stmt.kind = EXTERNAL;
        stmt.extDecl = parser_ExternalDeclaration();
    } else if(tok.kind == TOK_PROC || tok.kind == TOK_RECURSIVE || tok.kind == TOK_PUBLIC){
        stmt.kind = PROCEDURE;
        stmt.procDef = parser_ProcedureDefinition();
    } else {
        THROW_FROM_USER_CODE(
            ERROR,
            filename,
            tok.line,
            tok.column,
            "P0003",
            "unable to derive top-level kind from token '%s' (kind %d)",
            tok.lexeme,
            tok.kind
        );
        exit(1);
    }
    return stmt;
}
