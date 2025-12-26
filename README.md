<img src="./assets/Verity.png" style="border-radius: 20px;" />

# Verity Compiler

Verity is an AOT-compiled programming language, built for readability and easy interopability with existing tooling. It's 100% compatible with C, and builds to LLVM IR.

```
proc add(int a, int b) returns int {
    return a + b;
}

int x = 5;
mut int y = 10;
y = x + y;

print(add(x, y));
```

## Building

This project uses make to build. To build it, open this project in the root directory (in a terminal session) and run `$ make`.

## Language Design

Velerity follows a couple of patterns to make debugging easy, and programming safer.

[Look at the language spec](./notes/spec.md)