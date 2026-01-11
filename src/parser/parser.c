#include "parser.h"
#include "../diags/diagnostics.h"

Parser parser = {0};
Token tok = {0};
char* filename;

void parser_advance(){
    tok = parser.input.data[parser.pos * sizeof(Token)];
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

    int i = 0;
    while(stream.data[i * sizeof(Token)].kind != TOK_EOF){
        // parse top level stmt
        i ++;
    }
}
