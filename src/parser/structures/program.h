#pragma once
#include "../toplevel/toplevel.h"

typedef struct {
    TopLevelStatement* data;
    int count;
    int capacity;
    char* entrypoint;
} Program;

void Program_appendToplevel(Program* prog, TopLevelStatement stmt);
