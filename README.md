<!-- <img src="./assets/Verity.png" style="border-radius: 20px;" /> -->

# Verity Compiler

![License](https://img.shields.io/github/license/skzidev/verity?style=for-the-badge) ![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/skzidev/verity/c-cpp.yml?style=for-the-badge&branch=main&event=push)


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

To build and run this project, run these commands:
```sh
make
make run
```

**To run the test suite, run the following command:**

```sh
make test
```

Test files can be generated with the following command:

```sh
python tests/tools/gen.py {type} {module}
```

where `type` is one of the following: `unit,e2e,integration,golden`
and `module` is the name of a module in the src folder.

## Language Design

Verity follows a couple of patterns to make debugging easy, and programming safer.

[Look at the language spec](./spec/spec.md) or [View the grammar](./spec/grammar.ebnf)

## License

This project (VerityC, the C compiler implementation of Verity) alongside verity's specification is licensed under MIT. [See more](./LICENSE)