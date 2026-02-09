#pragma once

#include <stdio.h>
#include <stdbool.h>

#define no_code "UNTRACKED"

typedef enum {
    ERROR,
    WARNING,
    NOTE
} DiagnosticLevel;

extern void THROW(DiagnosticLevel lvl, char* code, char* fmt, ...);
extern void THROW_FROM_USER_CODE(DiagnosticLevel lvl, char* fname, int line, int col, char* code, char* fmt, ...);

extern void diag_assert_fail(const char* expr,
    const char *file,
    int line,
    const char *func
);

#define DiagnosticAssertion(cond) \
    do { \
        if (!(cond)) { \
            diag_assert_fail(#cond, __FILE__, __LINE__, __func__); \
        } \
    } while (0)
