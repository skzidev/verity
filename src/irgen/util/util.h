#pragma once
#include "../util/util.h"
#include <llvm-c-18/llvm-c/Types.h>

typedef enum {
    VIntegerType,
    VStringType,
    VFloatType,
    VCharType,
    VByteType,
    VBoolType
} Type;

Type StringTypeToVerityType(char*);
LLVMTypeRef VerityTypeToLLVMType(Type);
