#include <stdlib.h>
#include "./diagnostics.h"

void emitDiagnostic(const Diagnostic *d){
	if(d->level == ERROR)
	    printf("\e[1;31merror\e[0m [%s]: %s", d->code, d->message);
	else if(d->level == WARNING)
		printf("\e[1;33warning\e[0m [%s]: %s", d->code, d->message);
	else if(d->level == NOTE)
		printf("\e[1;34mnote\e[0m [%s]: %s", d->code, d->message);
    printf(" (%s:%d:%d)\n", d->filename, d->line + 1, d->column + 1);
    if(d->level == ERROR)
		exit(1);
}

void note(const char* msg, const char* fname, int line, int col){
	Diagnostic diag;
	diag.code = "UNTRACKED";
	diag.message = msg;
	diag.filename = fname;
	diag.level = NOTE;
	diag.line = line;
	diag.column = col;
	emitDiagnostic(&diag);
}

void errorAt(const char *code,
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