#include <stdbool.h>
#include "getopt.h"
#include "config.h"

CompilerOptions ParseArgs(int argc, char **argv){
    CompilerOptions opts;

    opts.version = false;
    opts.dumpAST = false;
    opts.silent = false;
    opts.help = false;
    opts.verbose = false;
    opts.dumpAST = false;
    opts.dumpToks = false;
    opts.inputFiles = NULL;

    int opt;
    while((opt = getopt_long(argc, argv, "vshb", long_opts, NULL)) != -1){
        switch(opt){
            case 'v':
                opts.version = true;
                break;
            case 's':
                opts.silent = true;
                break;
            case 'h':
                opts.help = true;
                break;
            case 'b':
                opts.verbose = true;
                break;
            case 1000:
                opts.dumpToks = true;
                break;
            case 1001:
                opts.dumpAST = true;
                break;
            case 1002:
                opts.isStandalone = true;
                break;
            case '?':
                exit(1);
        }
    }

    for(int i = optind; i < argc; i ++){
        // TODO make opts.inputFiles a valid array-like structure
        opts.inputFiles = &argv[i];
    }

    if(opts.inputFiles == NULL){
        // print help screen
        opts.help = true;
    }

    opts.shouldMute = opts.verbose && !opts.silent;

    return opts;
}
