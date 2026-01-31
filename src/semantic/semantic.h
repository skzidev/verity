#pragma once
#include "../parser/parser.h"
#include "./symbols/symbols.h"

extern ScopeStack stack;

Program semantics_enrich(Program ast);
