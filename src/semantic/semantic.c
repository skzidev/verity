#include "./semantic.h"
#include "toplevel/toplevel.h"
#include <stdio.h>

/**
 * Ensure the AST follows the language rules and add type information
 * to each AST node like expressions, and downconvert our size-agnostic types into LLVM's bit-width-dependent typing
 */
Program semantics_enrich(Program ast){
    for(int i = 0; i < ast.count; i ++){
        TopLevelStatement tp = ast.data[i];
        switch(tp.kind){
            case IMPORT:
                // parse symbols then place in symbol table
                printf("\tIMPORT STMT: %s, %s\n", tp.impt.package, tp.impt.ident);
            break;
            case PROCEDURE:
                printf("\tPROC DEF: %s \n", tp.procDef.ident);
                semantics_ProcDef(tp.procDef);
            break;
            case EXTERNAL:
                printf("\tEXTERNAL DECL: %s \n", tp.extDecl.kind == EXTERNAL_PROCDECL ? tp.extDecl.procDecl.ident : tp.extDecl.varDecl.ident);
            break;
        }
    }
    return ast;
}
