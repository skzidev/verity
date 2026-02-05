#pragma once
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "getopt.h"

#define helpScreen "usage: verity [OPTIONS] <input files>\n" \
    "ARGUMENTS:\n" \
    "\t<input files> - The input file paths\n" \
    "OPTIONS:\n" \
    "\t--version,-v - Display version information and exit\n" \
    "\t--silent,-s - Log nothing to stdout during compilation\n" \
    "\t--verbose,-b - Log extra information during compilation\n" \
    "\t--help,-h - Show this help screen and exit\n" \
    "\t--log-tokens - Log the tokens produced by the lexer\n" \
    "\t--log-ast - Log the AST produced by the parser\n"

#pragma GCC diagnostic ignored "-Wunused-variable"
static struct option long_opts[] = {
    { "version", no_argument, NULL, 'v' },
    { "silent", no_argument, NULL, 's' },
    { "help", no_argument, NULL, 'h' },
    { "verbose", no_argument, NULL, 'b' },
    { "log-tokens", no_argument, NULL, 1000 },
    { "log-ast", no_argument, NULL, 1001 },
    { "standalone", no_argument, NULL, 1002},
    { 0,0,0,0 }
};

typedef struct {
    bool version;
    bool silent;
    bool dumpAST;
    bool dumpToks;
    bool help;
    bool verbose;

    bool isStandalone;

    bool shouldMute;

    char** inputFiles;
} CompilerOptions;

CompilerOptions ParseArgs(int argc, char **argv);
