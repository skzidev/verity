#include "../parser.h"
#include "../../diags/diagnostics.h"
#include "../toplevel/toplevel.h"

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

void parser_Parse(TokenArray stream, char* fname){
    filename = fname;
    parser.input = stream;

    while(tok.kind != TOK_EOF){
        // parse top level stmt
        parser_TopLevelStatement();
    }
}
