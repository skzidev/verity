#include "../irgen.h"
#include "./desugarer.h"
#include "../../parser/toplevel/external.h"
#include <llvm-c-18/llvm-c/Core.h>
#include "../util/util.h"
#include <llvm-c-18/llvm-c/Types.h>

LLVMValueRef desugar_External(ExternalDeclaration extDecl){
    if(extDecl.kind == EXTERNAL_VARIABLEDECL)
        LLVMAddGlobal(module, VerityTypeToLLVMType(StringTypeToVerityType(extDecl.varDecl.type)), extDecl.varDecl.ident);
    //
}
