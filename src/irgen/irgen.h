#pragma once
#include "../parser/parser.h"
#include <llvm-c-18/llvm-c/Types.h>

extern LLVMModuleRef module;
extern LLVMBuilderRef builder;

void irgen_GenerateLLVMIR(Program* ast);
