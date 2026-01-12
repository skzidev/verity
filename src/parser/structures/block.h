#pragma once
#include "../statements/statement.h"

typedef struct Block {
    Statement* data;
    int count;
    int capacity;
} Block;

void block_append(Block* block, Statement stmt);
