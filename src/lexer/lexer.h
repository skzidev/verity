#pragma once

/**
 * A list of token types
 * These include keywords, punctuation, and operators.
 */
typedef enum {
	TOK_EOF,
	TOK_IDENT,
	TOK_INT,
	TOK_FLOAT,
	TOK_STRING,

	// Keywords
	TOK_PROC,
	TOK_RETURNS,
	TOK_THROWS,
	TOK_MUT,
	TOK_IF,
	TOK_ELSE,
	TOK_WHILE,
	TOK_FOR,
	TOK_RETURN,
	TOK_PROPAGATE,
	TOK_SKIP,
	TOK_BREAK,
	TOK_THROW,
	TOK_RECURSIVE,
	TOK_SWITCH,
	TOK_HANDLE,
	TOK_IMPORT,
	TOK_AS,
	TOK_EXTERNAL,

	// Symbols
	TOK_LPAREN, TOK_RPAREN,
	TOK_LBRACE, TOK_RBRACE,
	TOK_COMMA, TOK_SEMI,
	TOK_PERIOD,
	TOK_ASSIGN,
	TOK_PLUS, TOK_MINUS,
	TOK_STAR, TOK_SLASH
} TokenKind;

typedef struct {
	TokenKind kind;
	const char *lexeme;
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
 */
static Keyword keywords[] = {
	{"proc", TOK_PROC},
	{"propogate", TOK_PROPAGATE},
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
	{"external", TOK_EXTERNAL}
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