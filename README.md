<!-- <img src="./assets/Verity.png" style="border-radius: 20px;" /> -->

# Verity Compiler

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

## License

This project (VerityC, the C compiler implementation of Verity) alongside verity's specification is licensed under MIT. [See more](./LICENSE)