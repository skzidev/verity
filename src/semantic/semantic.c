#include "./semantic.h"
#include "symbols/symbols.h"
#include "toplevel/toplevel.h"
#include <stdio.h>

ScopeStack stack = {0};

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
            break;
            case PROCEDURE:
                // TODO put these in a different array to process later and just insert all top level decls into a new array which will be processed with all of the imported symbols intact
                // Additionally, place all procs into the scope before that
                semantics_ProcDef(tp.procDef);
            break;
            case EXTERNAL:
                Symbol externalSym = {0};
                bool isProcedure = tp.extDecl.kind == EXTERNAL_PROCDECL;
                externalSym.kind = isProcedure ? ProcedureSymbol : VariableSymbol;
                externalSym.definedLine = tp.extDecl.line;
                externalSym.ident = isProcedure ? tp.extDecl.procDecl.ident : tp.extDecl.varDecl.ident;
                externalSym.isMutable = false;
                if(!isProcedure)
                    externalSym.type = tp.extDecl.varDecl.type;
                ScopeStack_InsertSymbolAtLatestScope(&stack, externalSym);
            break;
        }
    }
    return ast;
}
