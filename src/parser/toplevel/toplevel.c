#include "./toplevel.h"
#include "../parser.h"
#include "external.h"
#include "import.h"
#include "procedures.h"
#include "../../diags/diagnostics.h"

ProcedureDefinition parser_ProcedureDefinition(){
    ProcedureDefinition stmt = {0};
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
    return stmt;
}

TopLevelStatement parser_TopLevelStatement(){
    TopLevelStatement stmt = {0};
    if(tok.kind == TOK_IMPORT){
        stmt.kind = IMPORT;
        stmt.impt = parser_ImportStatement();
    } else if(tok.kind == TOK_EXTERNAL){
        stmt.kind = EXTERNAL;
        stmt.extDecl = parser_ExternalDeclaration();
    } else if(tok.kind == TOK_PROC){
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
            tok.kind);
    }
}
