#include "./desugarer.h"
#include <llvm-c-18/llvm-c/Types.h>
#include "../irgen.h"

void desugar_Statement(Statement stmt){}

LLVMBasicBlockRef desugar_Block(Block block){

    for(int i = 0; i < block.count; i ++){

    }
}

LLVMValueRef desugar_Procedure(ProcedureDefinition procDef){

}
