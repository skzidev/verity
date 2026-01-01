#include <stdbool.h>
#include <stdio.h>
#include <getopt.h>
#include "config.h"
#include <bits/getopt_core.h>

#define helpScreen "usage: verity [OPTIONS] <input files>\n" \
    "ARGUMENTS:\n" \
    "\t<input files> - The input file paths\n" \
    "OPTIONS:\n" \
    "\t--version,-v - Display version information and exit\n" \
    "\t--silent,-s - Log nothing to stdout during compilation\n" \
    "\t--help,-h - Show this help screen and exit\n"

CompilerOptions ParseArgs(int argc, char **argv){
    CompilerOptions opts;

    opts.version = false;
    opts.dumpAST = false;
    opts.silent = false;
    opts.help = false;
    opts.inputFiles = NULL;

    int opt;
    while((opt = getopt_long(argc, argv, "vs:h", long_opts, NULL)) != -1){
        switch(opt){
            case 'v':
                opts.version = true;
                break;
            case 's':
                opts.silent = true;
                break;
            case 'h':
                printf("%s", helpScreen);
                exit(0);
                break;
            case '?':
                exit(1);
        }
    }

    for(int i = optind; i < argc; i ++){
        opts.inputFiles = &argv[i];
    }

    return opts;
}
