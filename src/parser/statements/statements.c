#include "../parser.h"
#include "../../diags/diagnostics.h"
#include "break.h"
#include "conditional.h"
#include "procedureCall.h"
#include "return.h"
#include "skip.h"
#include "statement.h"
#include "../structures/block.h"
#include "varasgn.h"
#include "vardef.h"

VariableDefinitionStatement parser_VariableDefinitionStatement(){
    VariableDefinitionStatement stmt = {0};
    if(tok.kind == TOK_MUT){
        stmt.isMutable = true;
        parser_expect(TOK_MUT);
    }
    stmt.type = tok.lexeme;
    parser_expect(TOK_IDENT);
    stmt.ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_ASSIGN);
    stmt.value = parser_expression();
    return stmt;
}

VariableAssignStatement parser_VariableAssignStatement(){
    VariableAssignStatement stmt = {0};
    stmt.ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_ASSIGN);
    stmt.value = parser_expression();
    return stmt;
}

ProcedureCall parser_ProcedureCall(){
    ProcedureCall call = {0};
    call.ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    parser_expect(TOK_LPAREN);
    call.params = parser_ExpressionList();
    parser_expect(TOK_RPAREN);
    return call;
}

IfStatement parser_IfStatement(){
    IfStatement stmt = {0};
    parser_expect(TOK_IF);
    parser_expect(TOK_LPAREN);
    stmt.condition = parser_Condition();
    parser_expect(TOK_RPAREN);
    stmt.block = parser_Block();
    return stmt;
}

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
        stmt.ifStmt = parser_IfStatement();
    } else if(tok.kind == TOK_MUT || (parser_peek(1).kind == TOK_ASSIGN)) {
        stmt.kind = VARIABLE_DEF;
        stmt.varDefineStatement = parser_VariableDefinitionStatement();
    } else if(tok.kind == TOK_IDENT && parser_peek(0).kind == TOK_ASSIGN) {
        stmt.kind = VARIABLE_ASGN;
        stmt.varAssignStatement = parser_VariableAssignStatement();
    } else if(tok.kind == TOK_IDENT && parser_peek(0).kind == TOK_LPAREN) {
        stmt.kind = PROCEDURE_CALL;
        stmt.procCall = parser_ProcedureCall();
    } else {
        THROW_FROM_USER_CODE(ERROR,
            filename,
            tok.line,
            tok.column,
            "P0003",
            "unknown statement kind");
    }
    parser_expect(TOK_SEMI);
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
    parser_expect(TOK_RETURN);
    stmt.value = parser_expression();
    return stmt;
}
