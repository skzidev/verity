/*
__     __        _ _
\ \   / /__ _ __(_) |_ _   _
 \ \ / / _ \ '__| | __| | | |
  \ V /  __/ |  | | |_| |_| |
   \_/ \___|_|  |_|\__|\__, |
                       |___/
    The Verity Compiler
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./lexer/lexer.h"
#include "./diags/diagnostics.h"

#include "./config/config.h"
#include "./globals.h"
#include "./parser/parser.h"
#include "parser/statements/statement.h"
#include "parser/toplevel/external.h"
#include "parser/toplevel/toplevel.h"

#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

#define COMPILER_VERSION "0.1"
#define LANGUAGE_VERSION "2025.0"

bool lexer_shouldLog = false;
bool parser_shouldLog = false;

FILE* fptr;
CompilerOptions opts;

char* readFileToString(char* fname){
    fptr = fopen(fname, "r");
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr) + 1;
    fseek(fptr, 0, SEEK_SET);

    char* fcontent = (char*) malloc(size * sizeof(char));
    fread(fcontent, 1, size, fptr);
    fcontent[size - 1] = '\0';
    return fcontent;
}

int main(int argc, char *argv[]){
	// Read args
	opts = ParseArgs(argc, argv);

    if(opts.version){
        printf("VerityC v%s (built %s at %s).\n" \
            "Verity spec v%s.\n" \
            "Licensed under MIT.\n",
            COMPILER_VERSION,
            BUILD_DATE,
            BUILD_TIME,
            LANGUAGE_VERSION
        );
        exit(0);
    }
    if(opts.help){
        printf("%s", helpScreen);
        exit(0);
    }

    lexer_shouldLog = opts.dumpToks;
    parser_shouldLog = opts.dumpAST;

	// Read main source file
	char* fcontent = readFileToString(opts.inputFiles[0]);

	// get tokens
	if(opts.verbose)
	    THROW(NOTE, no_code, "Starting Lexical Analysis");
    TokenArray tokenStream = lexer_tokenize(fcontent, opts.inputFiles[0]);
    if(opts.verbose)
        THROW(NOTE, no_code, "Lexical Analysis Complete");

    // parse token stream -> ast
    if(opts.verbose)
        THROW(NOTE, no_code, "Starting Structural Analysis");
    Program ast = parser_Parse(tokenStream, opts.inputFiles[0]);
    if(opts.verbose)
        THROW(NOTE, no_code, "Structural Analysis Complete");

    for(int i = 0; i < ast.count; i ++){
        TopLevelStatement tp = ast.data[i];
        switch(tp.kind){
            case IMPORT:
                printf("\tIMPORT STMT: \n");
            break;
            case PROCEDURE:
                printf("\tPROC DEF: %s \n", tp.procDef.ident);
                for(int i = 0; i < tp.procDef.block->count; i ++){
                    printf("\t\tSTATEMENT: %d\n", tp.procDef.block->data[i].kind);
                    switch(tp.procDef.block->data[i].kind){
                        case RETURN:
                            printf("\t\t\tRETURN\n");
                            break;
                        case PROCEDURE_CALL:
                            printf("\t\t\tPROC CALL: %s\n", tp.procDef.block->data[i].procCall.ident);
                            break;
                        case SKIP:
                            printf("\t\t\tSKIP:\n");
                            break;
                        case VARIABLE_ASGN:
                            printf("\t\t\tVARIABLE ASSIGN: %s\n", tp.procDef.block->data[i].varAssignStatement.ident);
                            break;
                        case VARIABLE_DEF:
                            printf("\t\t\tVARIABLE DEF: %s\n", tp.procDef.block->data[i].varDefineStatement.ident);
                            break;
                        case BREAK:
                            printf("\t\t\tBREAK:\n");
                            break;
                        case IF:
                        case ELSEIF:
                        case ELSE:
                            printf("\t\t\tCONDITIONAL\n");
                            break;
                    }
                }
            break;
            case EXTERNAL:
                printf("\tEXTERNAL DECL: %s \n", tp.extDecl.kind == EXTERNAL_PROCDECL ? tp.extDecl.procDecl.ident : tp.extDecl.varDecl.ident);
            break;
        }
    }

    return 0;
}
