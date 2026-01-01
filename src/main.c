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
#include <string.h>

#include "./lexer/lexer.h"
#include "./parser/parser.h"

#include "./config/config.h"

#include "diags/diagnostics.h"

#define COMPILER_VERSION "0.0.1"
#define LANGUAGE_VERSION "2025"

// Define required positional arguments
#define REQUIRED_ARGS \
    REQUIRED_STRING_ARG(input_file, "input", "Input file path") \

// Define optional arguments with defaults
#define OPTIONAL_ARGS \
	OPTIONAL_STRING_ARG(optimization, "0", "-O", "level", "The optimization level that should be used [0-3]") \
	OPTIONAL_STRING_ARG(target, "x86-64", "-t", "architecture", "The target CPU architecture [x86-64,aarch64]") \
	OPTIONAL_STRING_ARG(dump_ast, "./ast", "--dump-ast", "path", "Dump the AST to the provided file")

// Define boolean flags
#define BOOLEAN_ARGS \
    BOOLEAN_ARG(help, "-h", "Show this help screen") \
	BOOLEAN_ARG(version, "-v", "Display version information and exit") \
	BOOLEAN_ARG(silent, "-s", "Log nothing to stdout")

FILE* fptr;

int main(int argc, char *argv[]){
	// Read args
	CompilerOptions opts = ParseArgs(argc, argv);

    if(opts.version){
        printf("VerityC, version %s.\nImplementing Verity %s.\nLicensed under the MIT license.\n", COMPILER_VERSION, LANGUAGE_VERSION);
        exit(0);
    }

	// Read main source file
    fptr = fopen(opts.inputFiles[0], "r");
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char* fcontent = (char*) malloc(size * sizeof(char));
    fread(fcontent, 1, size, fptr);

	// Compile main file
    TokenArray tokenStream = lexer_tokenize(fcontent, opts.inputFiles[0]);
	parser_parse(&tokenStream, opts.inputFiles[0]);

    return 0;
}
