#include "./util.h"
#include <llvm-c-18/llvm-c/Core.h>
#include "../../globals.h"
#include "../../diags/diagnostics.h"

int GetTargetBitWidth(){
    DiagnosticAssertion(opts.target != NULL);
    if(strcmp(opts.target, "aarch64") == 0 || strcmp(opts.target, "x86_64") == 0){
        return 64;
    } else if(strcmp(opts.target, "i386") == 0 || strcmp(opts.target, "arm32")){
        return 32;
    } else {
        THROW(ERROR, "L0001", "Unsupported compilation target: '%s'", opts.target);
        exit(1);
    }
}

Type StringTypeToVerityType(char * type){
    DiagnosticAssertion(type != NULL);
    if(strcmp(type, "string") == 0){
        return VStringType;
    } else if(strcmp(type, "int") == 0){
        return VIntegerType;
    } else if(strcmp(type, "float") == 0){
        return VFloatType;
    } else if(strcmp(type, "char") == 0 || strcmp(type, "byte")){
        return VCharType;
    } else if(strcmp(type, "bool") == 0){
        return VBoolType;
    } else {
        THROW(ERROR, "L0003", "unknown type: '%s'", type);
        exit(1);
    }
}

LLVMTypeRef VerityTypeToLLVMType(Type type, bool* isMut){
    if(isMut == NULL) isMut = false;
    switch(type){
        case VStringType:
            if(isMut == false)
                return LLVMArrayType(LLVMInt8Type(), 0);
            else return LLVMPointerType(LLVMInt8Type(), 0);
        case VIntegerType:
            switch(GetTargetBitWidth()){
                case 64:
                    return LLVMInt64Type();
                case 32:
                    return LLVMInt32Type();
                default:
                    THROW(ERROR, "L0002", "Unsupported compilation target: '%s'", opts.target);
                    exit(1);
            };
            break;
        case VBoolType:
            return LLVMInt1Type();
        case VCharType:
        case VByteType:
            return LLVMInt8Type();
        case VFloatType:
            return LLVMFloatType();
        default: return LLVMInt64Type();
    }
}
