// This file is a test template, and should be modified before use in the testing suite
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../src/config/config.h"

bool lexer_shouldLog;

void test_argsparser(char* argv[], int argc, CompilerOptions expectedOpts){
	// Add your test logic here
	CompilerOptions recievedOpts = ParseArgs(argc, argv);

	printf(recievedOpts.inputFiles == NULL ? "Found no input files\n" : "Found input files\n");

	printf("asserting %b == %b\n", recievedOpts.silent, expectedOpts.silent);
	assert(recievedOpts.silent == expectedOpts.silent);
	printf("asserting %b == %b\n", recievedOpts.dumpAST, expectedOpts.dumpAST);
	assert(recievedOpts.dumpAST == expectedOpts.dumpAST);
	printf("asserting %b == %b\n", recievedOpts.help, expectedOpts.help);
	assert(recievedOpts.help == expectedOpts.help);
	printf("asserting %b == %b\n", recievedOpts.version, expectedOpts.version);
	assert(recievedOpts.version == expectedOpts.version);
	printf("asserting %b == %b\n", recievedOpts.verbose, expectedOpts.verbose);
	assert(recievedOpts.verbose == expectedOpts.verbose);

	// TODO ensure that input files list is the same
}

int main(){
    char* set1[] = { "./bin/verity", "-b", "-s", "./path/to/file.vty" };
    const int len1 = 4;
    CompilerOptions expected1 = {
        .version=false,
        .dumpAST=false,
        .help=false,
        .silent=true,
        .verbose=true
    };

    test_argsparser(set1, len1, expected1);
	printf("[PASS]: Test 'argsparser' Passed\n");
	return 0;
}
