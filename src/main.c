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
#include "./parser/parser.h"
#include "./semantic/semantic.h"

#include "./config/config.h"
#include "./globals.h"

#define BUILD_DATE __DATE__
#define BUILD_TIME __TIME__

#define COMPILER_VERSION "0.1"
#define LANGUAGE_VERSION "2025.0"

bool lexer_shouldLog = false;
bool parser_shouldLog = false;

FILE* fptr;
CompilerOptions opts;

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
    fptr = fopen(opts.inputFiles[0], "r");
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr) + 1;
    fseek(fptr, 0, SEEK_SET);

    char* fcontent = (char*) malloc(size * sizeof(char));
    fread(fcontent, 1, size, fptr);
    fcontent[size - 1] = '\0';

	// Compile main file
    TokenArray tokenStream = lexer_tokenize(fcontent, opts.inputFiles[0]);
    if(opts.shouldMute)
        THROW(NOTE, "UNTRACKED", "Lexical Analysis Complete");
    Program ast = parser_parse(&tokenStream, opts.inputFiles[0]);
    free(tokenStream.data);
    if(opts.shouldMute)
        THROW(NOTE, "UNTRACKED", "Parsing Complete");
    semantics_analyze(ast);

    return 0;
}
