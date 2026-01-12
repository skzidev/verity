#include "./parser.h"
#include "../diags/diagnostics.h"
#include "./toplevel/toplevel.h"
#include "./structures/program.h"

Parser parser = {0};
Token tok = {0};
char* filename;

void parser_advance(){
    tok = parser.input.data[parser.pos * sizeof(Token)];
}

Token parser_peek(int lookahead){
    return parser.input.data[(parser.pos + lookahead) * sizeof(Token)];
}

// TODO in Lexer, add TokenKind -> char* converter function

void parser_expect(TokenKind s){
    if(tok.kind == s) parser_advance();
    else THROW_FROM_USER_CODE(
        ERROR,
        filename,
        tok.line,
        tok.column,
        "P0001",
        "unexpected token '%s' (kind '%d', expected '%d')",
        tok.lexeme,
        tok.kind,
        s
    );
}

void Program_appendToplevel(Program *prog, TopLevelStatement stmt){
    if(prog->capacity == prog->count){
        prog->capacity = prog->capacity ? prog->capacity * 2 : 16;
        prog->data = (TopLevelStatement*) realloc(prog->data, sizeof(Expression) * prog->capacity);
    }
    prog->data[prog->count++] = stmt;
}

Program parser_Parse(TokenArray stream, char* fname){
    filename = fname;
    parser.input = stream;

    Program prog = {0};

    while(tok.kind != TOK_EOF){
        // parse top level stmt
        Program_appendToplevel(&prog, parser_TopLevelStatement());
    }
    return prog;
}
