#pragma once
#include "stdlib.h"

typedef struct {
    char* type;
    char* ident;
} Parameter;

Parameter parser_Parameter();

typedef struct {
    Parameter* data;
    int count;
    int capacity;
} ParameterList;

void ParameterList_push(ParameterList* list, Parameter item);

ParameterList parser_ParameterList();
