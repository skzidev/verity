#include "../../diags/diagnostics.h"
#include "../parser.h"
#include "expressionlist.h"
#include "parameterlist.h"
#include "identifierlist.h"
#include "block.h"
#include <complex.h>

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
        if(tok.kind == TOK_RPAREN)
            break;
        parser_expect(TOK_COMMA);
    }
    return paramList;
}

IdentifierList parser_IdentifierList(){
    IdentifierList l = {0};
    while(tok.kind == TOK_IDENT){
        IdentifierList_push(&l, tok.lexeme);
        parser_expect(TOK_IDENT);
        if(tok.kind != TOK_COMMA)
            break;
        parser_expect(TOK_COMMA);
    }
    return l;
}

void IdentifierList_push(IdentifierList* list, char* item){
    if(list->capacity == list->count){
        list->capacity = list->capacity ? list->capacity * 2 : 16;
        list->data = (char**) realloc(list->data, sizeof(char*) * list->capacity);
    }
    list->data[list->count++] = item;
}

void block_append(Block *block, Statement stmt){
    if(block->capacity == block->count){
        block->capacity = block->capacity ? block->capacity * 2 : 16;
        block->data = (Statement*) realloc(block->data, sizeof(Statement) * block->capacity);
    }
    block->data[block->count++] = stmt;
}

void ExpressionList_push(ExpressionList *list, Expression item){
    if(list->capacity == list->count){
        list->capacity = list->capacity ? list->capacity * 2 : 16;
        list->data = (Expression*) realloc(list->data, sizeof(Expression) * list->capacity);
    }
    list->data[list->count++] = item;
}

ExpressionList parser_ExpressionList(){
    ExpressionList list = {0};
    while(tok.kind != TOK_RPAREN){
        ExpressionList_push(&list, parser_expression());
        if(tok.kind == TOK_COMMA)
            parser_expect(TOK_COMMA);
        else if(tok.kind == TOK_RPAREN)
            break;
        else
            THROW_FROM_USER_CODE(ERROR,
               filename,
               tok.line,
               tok.column,
               "P0004",
               "unexpected token '%c' (kind %d)",
               tok.lexeme,
               tok.kind
            );
    }
    return list;
}

Block* parser_Block(){
    Block* block = calloc(1, sizeof(Block));

    parser_expect(TOK_LBRACE);
    while(tok.kind != TOK_RBRACE){
        block_append(block, parser_statement());
    }
    parser_expect(TOK_RBRACE);
    return block;
}
