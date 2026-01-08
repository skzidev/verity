#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#define helpScreen "usage: verity [OPTIONS] <input files>\n" \
    "ARGUMENTS:\n" \
    "\t<input files> - The input file paths\n" \
    "OPTIONS:\n" \
    "\t--version,-v - Display version information and exit\n" \
    "\t--silent,-s - Log nothing to stdout during compilation\n" \
    "\t--verbose,-b - Log extra information during compilation\n" \
    "\t--help,-h - Show this help screen and exit\n"

#pragma GCC diagnostic ignored "-Wunused-variable"
static struct option long_opts[] = {
    { "version", no_argument, NULL, 'v' },
    { "silent", no_argument, NULL, 's' },
    { "help", no_argument, NULL, 'h' },
    { "verbose", no_argument, NULL, 'b' },
    { "log-tokens", no_argument, NULL, 0 },
    { "log-ast", no_argument, NULL, 0 },
    { 0,0,0,0 }
};

typedef struct {
    bool version;
    bool silent;
    bool dumpAST;
    bool help;
    bool verbose;

    bool shouldMute;

    char** inputFiles;
} CompilerOptions;

CompilerOptions ParseArgs(int argc, char **argv);
