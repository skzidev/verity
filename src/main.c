/*
__     __        _ _         
\ \   / /__ _ __(_) |_ _   _ 
 \ \ / / _ \ '__| | __| | | |
  \ V /  __/ |  | | |_| |_| |
   \_/ \___|_|  |_|\__|\__, |
                       |___/ 
The Verity Programming Language
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"

// Define required positional arguments
#define REQUIRED_ARGS \
    REQUIRED_STRING_ARG(input_file, "input", "Input file path")

// Define optional arguments with defaults
#define OPTIONAL_ARGS \
	OPTIONAL_STRING_ARG(optimization, "0", "-o", "optimization", "The optimization level that should be used") \
	OPTIONAL_STRING_ARG(target, "x86-64", "-t", "target", "The target CPU architecture")

// Define boolean flags
#define BOOLEAN_ARGS \
    BOOLEAN_ARG(help, "-h", "Show help") \
	BOOLEAN_ARG(verbose, "-v", "Log extra information about compilation") \
	BOOLEAN_ARG(silent, "-s", "Log nothing to stdout")

#include "../include/easyargs.h"

FILE *fptr;

int main(int argc, char *argv[]){
	// Read args
	args_t args = make_default_args();
	if(!parse_args(argc, argv, &args) || args.help){
		print_help(argv[0]);
		return 1;
	}

    if(args.verbose)
        printf("reading input file '%s'\n", args.input_file);

	// Read main source file
    fptr = fopen(args.input_file, "r");
    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char* fcontent = (char*) malloc(size * sizeof(char));
    fread(fcontent, 1, size, fptr);

	// Compile main file5
    lexer_tokenize(fcontent, args.input_file);

    return 0;
}