#pragma once

typedef struct {
    char** data;
    int count;
    int capacity;
} IdentifierList;

void IdentifierList_push(IdentifierList* list, char* item);
IdentifierList parser_IdentifierList();
