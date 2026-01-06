// This file is a test template, and should be modified before use in the testing suite
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "../../src/config/config.h"

void test_argsparser(char* argv[], int argc, CompilerOptions expectedOpts){
	// Add your test logic here
	CompilerOptions recievedOpts = ParseArgs(argc, argv);

	assert(recievedOpts.silent == expectedOpts.silent);
	assert(recievedOpts.dumpAST == expectedOpts.dumpAST);
	assert(recievedOpts.help == expectedOpts.help);
	assert(recievedOpts.version == expectedOpts.version);
	assert(recievedOpts.silent == expectedOpts.silent);
	// TODO ensure that input files list is the same
}

int main(){
    char* set1[] = { "-b", "-s", "./path/to/file.vty" };
    const int len1 = 2;
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
