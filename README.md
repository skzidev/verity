<!-- <img src="./assets/Verity.png" style="border-radius: 20px;" /> -->

# Verity Compiler

 ![GitHub language count](https://img.shields.io/github/languages/count/skzidev/verity?style=for-the-badge)
 ![GitHub top language](https://img.shields.io/github/languages/top/skzidev/verity?style=for-the-badge)
 ![GitHub License](https://img.shields.io/github/license/skzidev/verity?style=for-the-badge)
 ![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/skzidev/verity/c-cpp.yml?style=for-the-badge&branch=main&event=push)


Verity is an AOT-compiled programming language, designed to maximize readability and DevEx.

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

Build requirements:
- Make
- Bash (or compatible; on Windows MSYS or Git Bash)
- GCC
- Cppcheck

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

where `type` is either: `unit`, or `e2e` and `module` is the name of a module in the src folder.

## Versioning

This project uses an epoch versioning system: `{EPOCH}.{COUNTER}` where `{EPOCH}` is the epoch, and `{COUNTER}` is the release number within that epoch. Every build tag gets its own version number.

The Verity language uses a CalVer system, however: `{YEAR}.{COUNTER}`, where `{YEAR}` is the gregorian year of the release, and `{COUNTER}` is the release number within that year.

## Roadmap

- [x] Args parser

- [x] Lexer

- [ ] Parser

- [ ] Semantic Analyzer

- [ ] Lowerer

- [ ] Integration with LLVM

## License

This project (VerityC, the C compiler implementation of Verity) alongside verity's specification is licensed under MIT. [See more](./LICENSE)
