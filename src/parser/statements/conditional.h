#pragma once
#include "../expressions/condition.h"
// forward declaration to prevent circular inclusion
typedef struct Block Block;

typedef struct {
    Condition condition;
    Block* block;
} IfStatement;
typedef struct {
    Condition condition;
    Block* block;
} ElseIfStatement;
typedef struct {
    Block* block;
} ElseStatement;

IfStatement parser_IfStatement();
ElseIfStatement parser_ElseIfStatement();
ElseStatement parser_ElseStatement();
