#include "../parser.h"
#include "parameterlist.h"
#include "identifierlist.h"

void ParameterList_push(ParameterList* list, Parameter item){
    if(list->capacity == list->count){
        list->capacity = list->capacity ? list->capacity * 2 : 16;
        list->data = (Parameter*) realloc(list->data, sizeof(Parameter) * list->capacity);
    }

    list->data[list->count++] = item;
}

Parameter parser_Parameter(){
    Parameter param = {0};
    if(tok.kind == TOK_MUT){
        param.isMutable = true;
        parser_expect(TOK_MUT);
    }
    param.type = tok.lexeme;
    parser_expect(TOK_IDENT);
    param.ident = tok.lexeme;
    parser_expect(TOK_IDENT);
    return param;
}

ParameterList parser_ParameterList(){
    ParameterList paramList = {0};
    while(tok.kind != TOK_RPAREN){
        ParameterList_push(&paramList, parser_Parameter());
        if(tok.kind != TOK_RPAREN)
            parser_expect(TOK_COMMA);
    }
    return paramList;
}

void IdentifierList_push(IdentifierList* list, char* item){
    if(list->capacity == list->count){
        list->capacity = list->capacity ? list->capacity * 2 : 16;
        list->data = (char**) realloc(list->data, sizeof(char*) * list->capacity);
    }
    list->data[list->count++] = item;
}
