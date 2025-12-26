#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// Function to extract a substring
char* extract_substring(const char *source, int start_index, int length) {
    // Allocate memory for the new substring (+1 for the null terminator)
    char *substring = (char*)malloc(length + 1);
    if (substring == NULL) {
        return NULL; // Handle memory allocation failure
    }

    // Copy the characters starting from the source + start_index
    strncpy(substring, source + start_index, length);

    // Manually add the null terminator at the end
    substring[length] = '\0';

    return substring;
}

typedef struct {
    const char* inputFile;
    const char* outputFile;

    const char* target;
    int optimization;
    bool silent;
    bool verbose;
} CompilerOptions;

CompilerOptions opts;

void config_set_opt(const char* flag){
    if(flag == "--verbose" || flag == "-v")
        opts.verbose = true;
    else if(flag == "--silent" || flag == "-s")
        opts.silent = true;
        /*
    else if(strncmp("--optimization=", flag, strlen("--optimization=")) == 0
        || strncmp("-o=", flag, strlen("-o=")) == 0)
        opts.optimization = atoi(((strchr(flag, '=') + 1) - flag));
    else if(strncmp("--target=", flag, strlen("--target=")) == 0
        || strncmp("-t=", flag, strlen("-t=")) == 0)
        opts.target = extract_substring(flag, (strchr(flag, '=') + 1) - flag, 3);*/
}