#include "../parser.h"
#include "../../diags/diagnostics.h"
#include "break.h"
#include "conditional.h"
#include "procedureCall.h"
#include "return.h"
#include "skip.h"
#include "statement.h"
#include "vardef.h"

Statement parser_statement(){
    bool requiresSemicolon = true;
    Statement stmt = {0};
    if(tok.kind == TOK_RETURN){
        stmt.kind = RETURN;
        stmt.returnStatement = parser_ReturnStatement();
    } else if(tok.kind == TOK_SKIP){
        stmt.kind = SKIP;
        stmt.skipStatement = parser_SkipStatement();
    } else if(tok.kind == TOK_BREAK){
        stmt.kind = BREAK;
        stmt.breakStatement = parser_BreakStatement();
    } else if(tok.kind == TOK_IF){
        stmt.kind = IF;
        // TODO stmt.ifStmt = parser_IfStatement();
    } else if(tok.kind == TOK_MUT || (parser_peek(2).kind == TOK_ASSIGN)) {
        stmt.kind = VARIABLE_DEF;
        // TODO stmt.varDefineStatement = parser_VariableDefinitionStatement();
    } else if(tok.kind == TOK_IDENT && parser_peek(0).kind == TOK_LPAREN) {
        stmt.kind = PROCEDURE_CALL;
        // TODO stmt.procCall = parser_ProcedureCall();
    } else {
        THROW_FROM_USER_CODE(ERROR,
            filename,
            tok.line,
            tok.column,
            "P0003",
            "unknown statement kind");
    }
    return stmt;
}

SkipStatement parser_SkipStatement(){
    parser_advance();
    SkipStatement stmt = {};
    return stmt;
}

BreakStatement parser_BreakStatement(){
    parser_advance();
    BreakStatement stmt = {};
    return stmt;
}

ReturnStatement parser_ReturnStatement(){
    ReturnStatement stmt = {0};
    parser_expect(TOK_RETURNS);
    stmt.value = parser_expression();
    return stmt;
}
