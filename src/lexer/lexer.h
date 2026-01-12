#pragma once

#include <stddef.h>
#include <stdbool.h>

extern bool lexer_shouldLog;

/**
 * A list of token types
 * These include keywords, punctuation, and operators.
 */
typedef enum {
    // C-style string termination byte, tokenized
	TOK_EOF,
	// Identifier
	TOK_IDENT,
	// Integer Literal
	TOK_INT,
	// Floating-point literal
	TOK_FLOAT,
	// String literal
	TOK_STRING,

	// Keywords
	// Procedure definition/declaration
	TOK_PROC,
	// Returns clause
	TOK_RETURNS,
	// Throws clause
	TOK_THROWS,
	// Mutablility marker
	TOK_MUT,
	// If keyword
	TOK_IF,
	// else keyword
	TOK_ELSE,
	// while keyword
	TOK_WHILE,
	// for keyword
	TOK_FOR,
	// return statement
	TOK_RETURN,
	// propagate keyword
	TOK_PROPAGATE,
	// skip statment
	TOK_SKIP,
	// break statement
	TOK_BREAK,
	// throw statement
	TOK_THROW,
	// recursive marker
	TOK_RECURSIVE,
	// switch statement
	TOK_SWITCH,
	// handle statement
	TOK_HANDLE,
	// import statement
	TOK_IMPORT,
	// as keyword
	TOK_AS,
	// external marker
	TOK_EXTERNAL,
	// public keyword
	TOK_PUBLIC,

	// Symbols
	TOK_LPAREN, TOK_RPAREN,
	TOK_LBRACE, TOK_RBRACE,
	TOK_COMMA, TOK_SEMI,
	TOK_PERIOD,
	TOK_ASSIGN,
	TOK_PLUS, TOK_MINUS,
	TOK_STAR, TOK_SLASH,
	TOK_RARR, TOK_LARR,
	TOK_EXCL,
	// Boolean literal
	TOK_BOOL,
	TOK_NULL
} TokenKind;

typedef struct {
	TokenKind kind;
	char *lexeme;
	int line;
	int column;
} Token;

typedef struct {
	Token *data;
	size_t count;
	size_t capacity;
} TokenArray;

typedef struct {
	const char* word;
	TokenKind kind;
} Keyword;

/**
 * Keyword string -> TokenKind lookup table
 * (diagnostic discarded because of testing includes)
 */
#pragma GCC diagnostic ignored "-Wunused-variable"
static Keyword keywords[] = {
    // Syntactic keywords
	{"proc", TOK_PROC},
	{"propagate", TOK_PROPAGATE},
	{"throws", TOK_THROWS},
	{"returns", TOK_RETURNS},
	{"return", TOK_RETURN},
	{"while", TOK_WHILE},
	{"if", TOK_IF},
	{"else", TOK_ELSE},
	{"mut", TOK_MUT},
	{"recursive", TOK_RECURSIVE},
	{"for", TOK_FOR},
	{"skip", TOK_SKIP},
	{"break", TOK_BREAK},
	{"switch", TOK_SWITCH},
	{"handle", TOK_HANDLE},
	{"throw", TOK_THROW},
	{"import", TOK_IMPORT},
	{"as", TOK_AS},
	{"external", TOK_EXTERNAL},
	{"public", TOK_PUBLIC},
	// Literal keywords
	{"true", TOK_BOOL},
	{"false", TOK_BOOL},
	{"null", TOK_NULL}
};

TokenKind lookup(const char* keyword);

/**
 * Append a token into a token array at the end, like javascript's array.prototype.push()
 */
void TokenArray_push(TokenArray* array, Token tok);

/**
 * Tokenize an input string and return a TokenArray
 */
TokenArray lexer_tokenize(char* input, char* fname);
