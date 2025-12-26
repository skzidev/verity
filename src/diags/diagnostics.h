#include <stdio.h>

#define ERROR(code, msg, file, tok) \
    error_at(code, msg, file, tok.line, tok.column);

typedef enum {
    ERROR,
    WARNING,
    NOTE
} DiagnosticLevel;

typedef struct {
    DiagnosticLevel level;
    const char* code;
    const char* message;
    const char* filename;
    int line;
    int column;
} Diagnostic;

void emitDiagnostic(const Diagnostic *d);
void emitDiagnostic(const Diagnostic *d){
    fprintf(stderr, "\e[1;31merror\e[0m [%s]: %s", d->code, d->message);
    fprintf(stderr, " (%s:%d:%d)\n", d->filename, d->line + 1, d->column + 1);
    exit(1);
}

void error_at(const char *code,
              const char *msg,
              const char *file,
              int line,
              int col);
void error_at(const char *code,
              const char *msg,
              const char *file,
              int line,
              int col){
    Diagnostic diag;
    diag.code = code;
    diag.message = msg;
    diag.filename = file;
    diag.level = ERROR;
    diag.line = line;
    diag.column = col;
    emitDiagnostic(&diag);
}
