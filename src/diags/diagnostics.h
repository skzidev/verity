#pragma once

#include <stdio.h>

#define THROW_ERROR(code, msg, file, tok) \
    errorAt(code, msg, file, tok.line, tok.column);
#define NOTE(msg, file, tok) \
    note(msg, file, tok.line, tok.column);

typedef enum {
    ERROR,
    WARNING,
    NOTE
} DiagnosticLevel;

/** A struct representing a diagnostic: an error, warning, or note given by the compiler based on the input */
typedef struct {
    DiagnosticLevel level;
    const char* code;
    const char* message;
    const char* filename;
    int line;
    int column;
} Diagnostic;

/** Prints a diagnostic to stdout or stderr */
extern void emitDiagnostic(const Diagnostic *d);

/** generates a note diagnostic based on the input given */
extern void note(const char* msg, const char* fname, int line, int col);

/** generates an error diagnostic based on the input given */
extern void errorAt(const char *code,
              const char *msg,
              const char *file,
              int line,
              int col);

/** generates an error diagnostic that has extra source code attached */
extern void fancyErrorAt(const char *code,
	const char* fname,
	int line,
	int col,
	const char* source);