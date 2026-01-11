#pragma once

typedef struct {
    char* package;
    char* ident;
} ImportStatement;

ImportStatement parser_ImportStatement();
