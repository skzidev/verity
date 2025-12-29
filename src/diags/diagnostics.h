#include <stdio.h>

#define THROW_ERROR(code, msg, file, tok) \
    error_at(code, msg, file, tok.line, tok.column);
#define NOTE(msg, file, tok) \
    note(msg, file, tok.line, tok.column);

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
	if(d->level == ERROR)
	    fprintf(stderr, "\e[1;31merror\e[0m [%s]: %s", d->code, d->message);
	else if(d->level == WARNING)
		fprintf(stderr, "\e[1;33warning\e[0m [%s]: %s", d->code, d->message);
	else if(d->level == NOTE)
		printf("\e[1;34mnote\e[0m [%s]: %s", d->code, d->message);
    fprintf(stderr, " (%s:%d:%d)\n", d->filename, d->line + 1, d->column + 1);
    if(d->level == ERROR)
		exit(1);
}

void note(const char* msg, const char* fname, int line, int col);
void note(const char* msg, const char* fname, int line, int col){
	Diagnostic diag;
	diag.code = "NOTE";
	diag.message = msg;
	diag.filename = fname;
	diag.level = NOTE;
	diag.line = line;
	diag.column = col;
	emitDiagnostic(&diag);
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
