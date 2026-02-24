#include "../irgen.h"
#include "./desugarer.h"
#include "../../parser/toplevel/external.h"
#include <llvm-c-18/llvm-c/Core.h>
#include "../util/util.h"
#include <llvm-c-18/llvm-c/Types.h>
#include <stdio.h>

LLVMValueRef desugar_External(ExternalDeclaration extDecl){
    if(extDecl.kind == EXTERNAL_VARIABLEDECL){
        printf("defining external '%s' of type %s\n", extDecl.varDecl.ident, extDecl.varDecl.type);
        LLVMValueRef global = LLVMAddGlobal(
            module,
            VerityTypeToLLVMType(
                StringTypeToVerityType(
                    extDecl.varDecl.type
                ),
                NULL
            ),
            extDecl.varDecl.ident
        );
        LLVMSetLinkage(global, LLVMExternalLinkage);
        return global;
    }

    IdentifierList StringTypes = (extDecl.procDecl.params);
    LLVMTypeRef* paramTypes = malloc(sizeof(LLVMTypeRef) * StringTypes.count);

    for(int i = 0; i < StringTypes.count; i ++){
        paramTypes[i] = VerityTypeToLLVMType(StringTypeToVerityType(StringTypes.data[i]), NULL);
    }

    LLVMValueRef extFunc = LLVMAddFunction(module, extDecl.procDecl.ident, LLVMFunctionType(VerityTypeToLLVMType(StringTypeToVerityType(extDecl.procDecl.returnType), NULL), paramTypes, extDecl.procDecl.params.count, false));
    LLVMSetLinkage(extFunc, LLVMExternalLinkage);
    return extFunc;
}
