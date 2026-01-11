# Verity 2026.1

This file details the Verity programming language in its 2026.1 version, including syntax and semantics.

## Goals

The Verity language has one main goal: to provide a powerful yet abstracted developer experience. Code should run efficiently, and programming should be safe and intuitive.

Verity should be able to support interop through the C ABI as well, and should include many safety features, including full memory safety.

## Syntax

Verity is designed to have a readible syntax, and is designed to offer the programmer extra information, without being verbose.

Procedures are declared with the `proc` keyword, as shown below
```
proc main() returns int {
  print("Hello, World!");
  return 0;
}
```

## Imports and modules

In Verity, each file is a module which can contain two types of symbols: public and private. All symbols, unless declared as public, are private by default.

Imports are written like this:
```
import "x" as y;
```
where x is the module name, and y is the identifier the module should be imported as.

Module source code is not directly copied and pasted into the main module, it is separately lexed and parsed, then public symbols are extracted and added to the symbol table, and those dependencies will be resolved at linktime.

## Types

Atomic types are defined by the compiler depending on the target architecture. Atomics include `int`, `char`, `bool`, `float`, `string`, `void`, and are all defined by the compiler, seperate from the stdlib.

Additionally, `error` types are thrown, where `error` is defined as a trait in the stdlib, which will be entirely copy/pastable if need be.

## The Standard Lib

The standard libraary includes a bunch of types that are useful for modern development like web servers, web clients, file system utilities, picture manipulation utilities, sound manipulation utilities. 

## Safety

Memory safety is achieved through automatic reference counting, 

## Notes

- Functions are values
- Copies are performed by value by default.
  - References (Pointers) are explicit
- Imports are NOT injected into the code directly. They are separately parsed, then the public symbols are added to the symbol table. The files are then compiled separately and linked together.
