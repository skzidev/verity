#include <llvm-c-18/llvm-c/Core.h>
#include <stdio.h>
#include <llvm-c-18/llvm-c/Analysis.h>
#include <llvm-c-18/llvm-c/Types.h>
#include "irgen.h"
#include "../semantic/toplevel/toplevel.h"
#include "../globals.h"
#include "desugar/desugarer.h"

LLVMModuleRef module;
LLVMBuilderRef builder;

void irgen_GenerateLLVMIR(Program* ast){
    for(int i = 0; i < ast->count; i ++){
        switch(ast->data[i].kind){
            case PROCEDURE:
                desugar_Procedure(ast->data[i].procDef);
                break;
            case IMPORT:
                //
                break;
            case EXTERNAL:
                desugar_External(ast->data[i].extDecl);
                break;
        }
    }
}
