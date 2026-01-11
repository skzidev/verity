#include "../stmtParser/stmtParser.h"
#include "../../parser/parser.h"

/*
 * This is supposed to be a more stable way of checking input, based on the approach that I used to test the lexer in my lexer's unit tests.
 * The user of this function can provide an array of token types, and if all of the types match, it returns `true`, otherwise false.
 */

bool doesAccept(Parser* parser, TokenKind pattern[]){
    bool rollingTruth = true;
    int numOfItems = (sizeof(&pattern)/sizeof(pattern[0]));
    for(int i = 0; i < numOfItems; i ++){
        rollingTruth &= (parser->tokens->data[parser->pos + i].kind == pattern[i]);
    }
    return rollingTruth;
}
