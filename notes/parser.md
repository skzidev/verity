# Parser Notes

The parser takes input, a `TokenArray` from the lexer, and generates an Abstract Syntax Tree (AST) from it. As discussed in the [Lexer Notes](./lexer.md), Lexers output an array of `Token`s, which are just that. Simple, one-dimensional arrays. Parsers, as aforementioned, output Abstract syntax trees, which are slightly different.

ASTs take tokens and organize them into grammar-defined structures, _Non-terminals_, which are composed of either other non-terminals, or _Terminals_. Terminals are indivisible, whereas non-terminals are, as aforementioned, made up of other grammar structures, either terminals or nonterminals.

This parser is a **Recursive Descent Parser**. That means functions are declared to match each non-terminal, those functions call each other, recursing when needed, to divide themselves down until only terminals remain.

## Implementation Details

Each nonterminal defined in the language grammar (which is written in EBNF) is given a function and cooresponding struct.

## Closing

...
