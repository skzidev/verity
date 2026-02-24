#include <stdbool.h>
#include <stdio.h>
#include "getopt.h"
#include "config.h"

CompilerOptions ParseArgs(int argc, char **argv){
    CompilerOptions opts = {0};

    int opt;
    while((opt = getopt_long(argc, argv, "vshbt", long_opts, NULL)) != -1){
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
            case 1003:
                opts.dumpIR = true;
                break;
            case 't':
                opts.target = optarg;
                break;
            case '?':
                exit(1);
        }
    }

    // TODO get current architecture
    if(opts.target == NULL) opts.target = "x86_64";

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
