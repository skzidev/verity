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
} SemanticType;

SemanticType getTypeFromString(char* str);
IdentifierList semantics_StripTypesFromParameterList(ParameterList* list);
