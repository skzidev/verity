#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

static struct option long_opts[] = {
    { "version", no_argument, NULL, 'v' },
    { "silent", no_argument, NULL, 's' },
    { "help", no_argument, NULL, 'h' },
    { 0,0,0,0 }
};

typedef struct {
    bool version;
    bool silent;
    bool dumpAST;
    bool help;

    char** inputFiles;
} CompilerOptions;

CompilerOptions ParseArgs(int argc, char **argv);
