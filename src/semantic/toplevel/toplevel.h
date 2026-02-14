#pragma once
#include "../../parser/parser.h"

ProcedureDefinition semantics_ProcDef(ProcedureDefinition tp);
ProcedureDefinition semantics_ProcDecl(ProcedureDefinition tp);

typedef enum {
    StringType,
    IntType,
    FloatType,
    BoolType,
    NullType,
    CompositeType
} Type;

Type getTypeFromString(char* str);
