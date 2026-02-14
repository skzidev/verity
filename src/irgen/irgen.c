#include <llvm-c-18/llvm-c/Core.h>
#include <stdio.h>
#include <llvm-c-18/llvm-c/Analysis.h>
#include <llvm-c-18/llvm-c/Types.h>
#include "irgen.h"
#include "../semantic/toplevel/toplevel.h"

LLVMModuleRef file;
LLVMBuilderRef builder;

LLVMTypeRef verityTypeToLLVMType(Type verityType){
    switch(verityType){
        case StringType:
            return LLVMArrayType(LLVMInt8Type(), 0);
        case IntType:
            // TODO adapt to host architecture size
            return LLVMInt64Type();
        case FloatType:
            return LLVMFloatType();
        case BoolType:
            return LLVMInt1Type();
        case NullType:
            return LLVMInt64Type();
        case CompositeType:
            // TODO add composited type
            return LLVMInt64Type();
    }
    return LLVMInt64Type();
}

LLVMTypeRef ParameterToLLVMType(Parameter parameter){
    return verityTypeToLLVMType(getTypeFromString(parameter.type));
}

void GenerateLLVMStatementCode(Statement s, LLVMValueRef func){
    switch(s.kind){
        case VARIABLE_DEF:
            //
            LLVMBuildAlloca(builder, verityTypeToLLVMType(getTypeFromString(s.varDefineStatement.type)), s.varDefineStatement.ident);
        break;
    }
}

void irgen_GenerateLLVMIR(Program* ast){
    file = LLVMModuleCreateWithName(filename);
    builder = LLVMCreateBuilder();

    for(int i = 0; i < ast->count; i ++){
        TopLevelStatement tp = ast->data[i];
        switch(tp.kind){
            case IMPORT:
            break;
            case EXTERNAL:
                ExternalDeclaration extDecl = tp.extDecl;
                if(extDecl.kind == EXTERNAL_PROCDECL){
                    LLVMTypeRef paramTypes[extDecl.procDecl.params.count];
                    for(int i = 0; i < extDecl.procDecl.params.count; i ++){
                        paramTypes[i] = verityTypeToLLVMType(getTypeFromString(extDecl.procDecl.params.data[i]));
                    }
                    LLVMTypeRef func_type = LLVMFunctionType(
                            verityTypeToLLVMType(
                                getTypeFromString(
                                    extDecl.procDecl.returnType
                                )
                            ),
                            paramTypes,
                            extDecl.procDecl.params.count,
                            0
                    );
                    LLVMValueRef func = LLVMAddFunction(file, extDecl.procDecl.ident, func_type);
                    LLVMSetLinkage(func, LLVMExternalLinkage);
                } else {
                    LLVMValueRef c = LLVMAddGlobal(file, verityTypeToLLVMType(getTypeFromString(extDecl.varDecl.type)), extDecl.varDecl.ident);
                    LLVMSetLinkage(c, LLVMExternalLinkage);
                }
            break;
            case PROCEDURE:
                ProcedureDefinition procDef = tp.procDef;
                LLVMTypeRef paramTypes[procDef.params.count];
                for(int i = 0; i < procDef.params.count; i ++){
                    paramTypes[i] = ParameterToLLVMType(procDef.params.data[i]);
                }
                LLVMTypeRef func_type = LLVMFunctionType(
                        verityTypeToLLVMType(
                            getTypeFromString(
                                procDef.returnType
                            )
                        ),
                        paramTypes,
                        procDef.params.count,
                        0
                );
                int VariableCounter = 0;
                LLVMValueRef func = LLVMAddFunction(file, procDef.ident, func_type);
                LLVMContextRef context = LLVMGetModuleContext(file);
                LLVMBasicBlockRef entry_block = LLVMAppendBasicBlockInContext(context, func, "entry");
                LLVMPositionBuilderAtEnd(builder, entry_block);
                for(int i = 0; i < procDef.block->count; i ++){
                    Statement s = procDef.block->data[i];
                    GenerateLLVMStatementCode(s, func);
                }
            break;
        }
    }

    char* IRString = LLVMPrintModuleToString(file);
    if (IRString) {
        printf("--- LLVM IR ---\n%s\n---------------\n", IRString);
        // Deallocate the string allocated by LLVM
        LLVMDisposeMessage(IRString);
    } else {
        fprintf(stderr, "Failed to get LLVM IR string\n");
    }
}
