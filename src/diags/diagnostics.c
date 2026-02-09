#include "./diagnostics.h"
#include <stdarg.h>
#include <stdlib.h>

__attribute__((format(printf, 3, 4)))
void THROW(DiagnosticLevel level, char* code, char* fmt, ...){
    va_list args;
    va_start(args, fmt);
    if(level == ERROR){
	    printf("\e[1;31merror\e[0m [%s]: ", code);
    } else if(level == WARNING){
		printf("\e[1;33mwarning\e[0m [%s]: ", code);
    } else if(level == NOTE){
		printf("\e[1;34mnote\e[0m [%s]: ", code);
    }
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
}

__attribute__((format(printf, 6, 7)))
void THROW_FROM_USER_CODE(DiagnosticLevel level, char* fname, int line, int col, char* code, char* fmt, ...){
    va_list args;
    va_start(args, fmt);
    if(level == ERROR){
	    printf("\e[1;31merror\e[0m [%s]: ", code);
    } else if(level == WARNING){
		printf("\e[1;33mwarning\e[0m [%s]: ", code);
    } else if(level == NOTE){
		printf("\e[1;34mnote\e[0m [%s]: ", code);
    }
    vprintf(fmt, args);
    printf(" (%s:%d:%d)\n", fname, line + 1, col + 1);
    va_end(args);
}

void diag_assert_fail(const char* expr,
    const char *file,
    int line,
    const char *func
){
    THROW(ERROR, "UNTRACKED", "compiler assertion \"%s\" failed in '%s' (%s:%d)",
        expr, func, file, line);
    THROW(NOTE, "UNTRACKED", "^ this is likely an issue with this compiler, not your program.");
    exit(1);
}
