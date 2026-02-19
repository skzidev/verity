#pragma once
#include <llvm-c-18/llvm-c/Core.h>
#include "../../parser/parser.h"
#include <llvm-c-18/llvm-c/Types.h>

LLVMValueRef desugar_Procedure(ProcedureDefinition);
LLVMValueRef desugar_External(ExternalDeclaration);
