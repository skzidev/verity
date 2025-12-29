# Lexer Notes

The lexer has one main goal. It is the first part of the compilation process, and its job is to take the input (in the form of a string), and convert it into "tokens", which are fed into the parser to later generate an AST.

The lexer is fairly simple. It defines some token types. In our case, we declare them in an enum with a total of 35 types. These types are listed below. The lexer loops through each character in the input, and assigns a different token type for each character. In the case that it encounters an alphanumeric character, however, it consumes all alphanumeric characters proceeding it, so that each "word", if you will, will be in a singular token. 

### General
- `TOK_EOF`
- `TOK_IDENT`
- `TOK_INT`
- `TOK_FLOAT`
- `TOK_STRING`

### Keywords
- `TOK_PROC`
- `TOK_RETURNS`
- `TOK_THROWS`
- `TOK_MUT`
- `TOK_IF`
- `TOK_ELSE`
- `TOK_WHILE`
- `TOK_FOR`
- `TOK_RETURN`
- `TOK_PROPAGATE`
- `TOK_SKIP`
- `TOK_BREAK`
- `TOK_THROW`
- `TOK_RECURSIVE`
- `TOK_SWITCH`
- `TOK_HANDLE`
- `TOK_IMPORT`
- `TOK_AS`

### Symbols
- `TOK_LPAREN`
- `TOK_RPAREN`
- `TOK_LBRACE`
- `TOK_RBRACE`
- `TOK_COMMA`
- `TOK_SEMI`
- `TOK_PERIOD`
- `TOK_ASSIGN`
- `TOK_PLUS`
- `TOK_MINUS`
- `TOK_STAR`
- `TOK_SLASH`

## Implementation Details

Because C does not have dynamically-resized arrays, a `TokenArray` struct was created, and a `TokenArray_push()` method was created to append an item to the end of a `TokenArray`. The `TokenArray_push()` function handles dynamic memory allocation and reallocation by itself.

Tokens are defined with the `Token` struct, and they contain a string, the textual origin of that token, alongside some metadata like position in the input document, and a type used during other stages of compilation.

A lookup table was also used to map keyword strings onto keyword token types, so that each keyword gets its own lexer type.

## Errors

| Error code | Fix |
|------------|-----|
| `L0001` | Place the closing quote of the offending string before a newline |
| `L0002` | Find the token at the specified line/column, then find a way to remove it. |

## Closing

Lexers seem simple, and that's because they are, but they are crucial for compilation. Compilers must deal a lot in string manipulation, and lexers divide one long string into smaller substrings, dubbed tokens, that are used later in the compilation process. 