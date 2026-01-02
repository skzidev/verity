#pragma once

#include <stdio.h>

typedef enum {
    ERROR,
    WARNING,
    NOTE
} DiagnosticLevel;

extern void THROW(DiagnosticLevel lvl, char* code, char* fmt, ...);
extern void THROW_FROM_USER_CODE(DiagnosticLevel lvl, char* fname, int line, int col, char* code, char* fmt, ...);
