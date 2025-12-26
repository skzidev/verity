# Verity Language Specification

# 1.0 Introduction

This document describes the Verity Programming Langauge in its 0.0.1 version, including its syntax, semantics and philosophy. This document is not meant to serve as a tutorial. This document is meant to be used as a guide for developers of a Verity compiler or interpreter. Because this is version 0.0.1, many things are subject to change in later versions of the specification and the compiler derived from it.

### 1.1 Goals

Verity seeks to recreate the philosophy and design of Java, using the low-levelness of C and C++. Java's requirement of a runtime means that it is ill-used on embedded systems and low-level systems, whereas compiled languages all build to a target, allowing them to be recompiled to run in several different environments at the lowest level possible. For this reason, Verity was created.

Verity differs from other languages because its goal is to get as close as it can to being the easiest, yet most powerful language to use. It aims to give the fine-grained control over everything the computer does to the programmer, while also giving the programmer the higher-level tools to make writing code simpler. That way, developers can pick what they want to heavily optimize, and what a pre-existing, standard implementation is good enough for.

### 1.2 Philosophy

Programming languages, ultimately, are abstractions. Verity's goal is to be a low-level language, constructed with high-level thought. Pointers are often not found in high-level languages. They are often abstracted away. Pointers though, are very useful. They give you the ability to modify memory. While that can cause errors like segmentation faults, Verity's exception system is there to catch that.

### 1.3 Non-goals

Verity does **not** aim to support compiletime metaprogramming, support macros, or be runtime dependent (in spec). 

# 2.0 Lexical Analysis

Like in most languages, Verity programs are read by Lexers (otherwise known as Lexical Analyzers or Tokenizers). The lexer recieves input as source code, and outputs a list of tokens derived from the string input. Lexers commonly read one character at a time, and the character that it sees determines the type of the next token. Tokens can be expressions, identifiers, keywords, or syntactic structure. 

### 2.1 Keywords

Verity interpreters and compilers, within accordance with this document, must support the following keywords:

| Keyword | Meaning |
|---------|---------|
| `proc`  | Marks a _procedure_ declaration |
| `returns` | Marks a _return clause_ on a procedure declaration |
| `throws` | Marks a _error clause_ on a procedure declaration
| `mut` | Marks the explicit mutabilization of a variable |
| `if` | Marks an _if statement_ |
| `else` | Marks an else clause on an if statement |
| `while` | Marks a while loop |
| `return` | Marks a return |
| `propogate` | Marks an explicit propogation allowance |
| `throw` | Marks an exception throw statement |
| `handle` | Marks an exception handling statement |
| `recursive` | Marks a procedure as recursive |
| `for` | Marks a for loop |
| `switch` | Marks a switch statement |
| `break` | Marks a break statement |
| `skip` | Marks a skip or "continue" statement | 

### 2.2 Identifiers

Identifiers and names in Verity must be alphanumeric and must be in the ASCII encoding. They can include letters A-Z both uppercase and lowercase, and can support digits 0-9. They cannot, however, contain punctuation, non-ASCII letters, or whitespace. Verity is case-sensitive with identifiers. 

### 2.3 Literals

Verity compilers and interpreters must support string, character, boolean, and numeric literals.

### 2.4 Operators

Verity compilers and interpreters must support these operators:

| Operator | Operation |
|---------|---------|
| + | Addition |
| * | Multiplication |
| - | Subtraction |
| / | Division |
| % | Modulus |
| = | Assignment |
| == | Equality |
| \| | Bitwise OR |
| \|\| | Logical OR |
| & | Bitwise AND |
| && | Logical AND |
| ++ | Increment (postfix only) |
| -- | Decrement (postfix only) |
| += | Increase by |
| -= | Decrease by |

### 2.5 Whitespace

Verity programs can include indentation with tabs, newlines or spaces, with no constraints put on how much whitespace may be there. The lexer strips out extra whitespace when it its being lexed, so its inclusion is purely for developer readability. The only whitespace needed is the spaces between keywords.

There is no enforced whitespace standard, but I use 4 space-sized tabs for clear visual separation (Except VSCode's default is spaces, so I use "emulate tabs" there)

### 2.6 Comments

Verity single-line comments are marked using '//'. All text proceeding that marker should be ignored by the lexer or parser.

Multi-line comments are marked using /= at the start and =/ at the end. This is kind of unconventional, but I do it because it's faster and easier to type a '=' than a '*'.

# 3.0 Semantics

### 3.1 Expressions

Expressions in Verity can be evaluated with 3 things. Expressions have two sides, a left and right side. Then, expressions have an operator. Operators, as defined in § 2.4, dictate what operation the left and right sides undergo.

In terms of evaluation, expressions are evaluated with a modified version of PEMDAS, in this case PMDAS (Parentheses, Multiplication, Division, Addition, Subtraction), from left to right.

Mixed type operations will be supported for numerical types (ints and floats, for example) only. Type promotion, however, is **implicit**. 

**Note: Assignment is considered a statement, not an expression**

### 3.2 Types

Verity is statically typed, and by default has a few types that can be used.

- String
- Integer
- Floating Point Decimal
- Boolean
- Array
- Dictionary

The type of a variable must be included in its declaration, as shown below

`string mode = "ON";`

Type inferencing is not supported in this version of the Verity standard. When defining a variable, its type must be explicitly declared.

Type casting must be explicit by putting the desired type in parentheses, as shown here.`string numberString = (string) 15;`. If a cast does not exist from the source type to the target type, the compiler will throw an error describing the issue.

Numerical types (integer, float) are of a target-determined size. In the case of an interpreter, the size should be the size based on the host machine. For instance, if a program is built for target `arm32`, the size of the type should be 32 bits, but if it is being built for `aarch64`, the size of the type should be 64 bits. All numerical types are currently signed, and cannot, within this version of the specification, be declared as unsigned.

### 3.3 Control Flow

On top of expressions, there are also block statements. Statements of this type contain a piece of code that is executed at the discretion of the statement which the block belongs to. For instance, an if statement's block will execute in the case that its condition evaluates to `true`. Block statements have their own scope, which can be used to define variables.

The following control flow statements should be implemented:

- `if`
- `else if`
- `else`
- `for`
- `while`
- `switch`

Additionally, `break` can be used in loops to terminate the loop early, and `skip` can be used to skip ahead to the next iteration.

Control starts at the entrypoint of a program, which is a procedure. By default, the entrypoint should be the integer-returning procedure named `main`, unless otherwise specified by the user of the implementation.

For loops are **not range-based**, as range-based loops give you less control over the exact number of iterations, so for loops use a more standard syntax of for(definition; condition; update). 

### 3.4 Memory management 

Memory in Verity programs does not need to be explicitly allocated or freed. Depending on implementation, Verity compilers may insert commands into the program to allocate and free memory, but the exact implementation details can be decided by the makers of the specific compiler or interpreter.

If you are developing an implementation for Verity, First of all, thank you, but don't worry about how you manage memory. You can use any system you want. For VerityC, I will be using reference counting, but do whatever feels best.

Additionally, the assignment operator **always copies data**. If a variable is assigned to another, the data will be assigned by value. Additionally, parameters are passed passed by value by default. To be passed by reference, the variable should pass a pointer instead.

Finally, variables are immutable by default. In order to be made mutable, the variable must be defined with the `mut` keyword in its definition. Function parameters additionally can be made mutable with the `mut` keyword. The `mut` keyword applies to _the variable which it is used on_, (the binding) and mutability is not transitive. Once a variable is marked mutable or immutable, that status is to remain for the lifetime of the variable. For instance,

```
mut int myInt = 5; // Mutable
int myNewInt = myInt; // Immutable
```

Variables which have been marked as mutable can be reassigned. Variables which have not been marked as mutable with the `mut` keyword cannot be reassigned- any attempt to do so should result in an error from the implementation of Verity that is being used.

Additionally, variables are scoped to blocks. Block shadowing is supported. Loop variables are treated as if they were in the loop- created on enter and destroyed on exit.

### 3.5 Procedures

Procedures, interchangably known as functions, can be declared using the `proc` keyword. Function declarations must include the following information:

- Name
- Argument/parameter list (Name & type of parameters)
- Return type

If a function contains a statement which could throw an error, it must also declare

- Thrown exceptions

If, as will be discussed shortly, procedure is recursive, it must also declare so with the "recursive" keyword before the "proc" keyword.

An example of a procedure declaration is shown below

```
proc readConfigurationFile(string path, string mode) returns string throws FileNotFoundError {}
```

Here, a procedure named `readConfigurationFile` is defined, which accepts two string arguments. Firstly, it accepts a string `path`, and secondly, it accepts a string `mode`. This procedure also declares that it returns a `string` type, and declares that it throws a `FileNotFoundError`.

In order to be recursive, functions must be explicitly declared as able to do so with the `recursive` keyword. A verity compiler or interpreter should not allow a procedure to recurse unless it is explicitly declared by the programmer as able to do so. Additionally, if a non-recursive procedure is declared as recursive, a warning should be thrown.

### 3.6 Aynchronous Programming

Asynchronous programming is not supported in this version of the standard.

### 3.7 Pointers

Pointers to variables can be made by placing a '*' character in front of its identifier. For example, if I declare an integer, I can get a pointer to it with `*myInteger`. Pointers can be dereferenced with an '&' character in front of it. So, `&(*myInteger)` will return myInteger.

Simply put, `*x` is **address-of**, `&y` is **dereference**

Additionally, pointers are immutable, as they are considered quasi-properties of objects, not variables themselves. Pointer arithmetic is supported, and can be used for manual memory management. Doing something like what is shown below should lead to an error, as discussed.

```
*ptr = 5; // should lead to an ImmutableMutationError, just like an attempt to mutate an immutable variable
```

Under the hood, pointers are just memory addresses with a coat on, so they should be tretaed as numbers. Therefore, pointer arithmetic is supported. Pointers should carry additional metadata, however, like _base address_, _allocation size_, and _allocation lifetime_.
This metadata should be used by the implementation to throw `MemoryAccessError`s before the program pokes into memory at a place it shouldn't and hits undefined behavior.
If the pointer is created in foreign code or by a raw address, then the allocation size would be unknown, and the compiler can't catch the error. In that case, arithmetic on that pointer is implementation-defined. This tracking however, should be internal to the compiler/interpreter and is not observable by user code.

Implementations may reject programs that return or store pointers whose lifetime does not outlive the pointer's usage.

Additionally, dereferencing a null pointer will lead to a defined `NullPointerError`.

### 3.8 Arrays & Dictionaries

Arrays and dictionaries are types of variables that allow multiple values to be stored within one variable. In Verity, Arrays and Dictionaries are dynamically sized. Dictionaries, which should be implemented as hash maps, are not reliably ordered by this specification. Any ordering consistency is a byproduct of the compiler's design, not this specification. Additionally, arrays and dictionaries store values, not references or pointers.

# 4.0 Errors & Exceptions

### 4.1 Error specification

Verity uses exceptions to handle errors, as they allow developers to easily debug programs. Exceptions can be handled through handle statements, as shown below, or they can be propogated up the call stack. In the event that an error is propogated up to the entrypoint of the program, the error must be logged to the console, through either `stdout` or preferably `stderr`, alongsde the error's message. Following the error output to the console, the program must be terminated.

If a procedure may produce an error, it must either declare that error in its `throws` clause, or handle it with a `handle` statement.

When calling a procedure that throws one or more errors, those error(s) must be either propogated or handled.

Error must be descended of the `Error` class  to be implemented in the standard library of an implementation.

Additionally, compound expressions throw all of the errors contained in its subexpressions.

### 4.2 Catching Errors

Errors can be handled through handle statements, as demonstrated below.

```
import "std/fs" as fs;

proc handler(err) returns string {
    return "{ 'error': 'No config file exists' }";
}

proc main(string[] args) returns int {
    string content = fs.readText("../config.txt")
        handle FileNotFoundError => handler();
    ...
}
```

Handle statements can be attached to any expression or statement, and they catch errors. Note that handle statements can catch plain `Error`s to catch any kind of error, but it's good practice to be specific so any other problems can be fixed. Handle specifically binds to the nearest preceding expression unless parentheses override this. Put in other words,

```
foo() + bar()
	handle Error => callback();
```

"Does handle apply to foo() + bar(), or just bar()": handle applies to bar() unless parenthesis override that.

### 4.3 Propogating errors

Errors can be propogated up the call stack using the `propogate` keyword. For example:

```
import "std/fs" as fs;

proc main(string[] args) returns int {
    string content = propogate fs.readText("../config.txt");
}
```

Implementations should return an error and refuse to exeucte or compile the code if an error is propogated and not handled further up the call stack.

### 4.4 Notes on errors which are unhandled and unpropogated

Implemenations of Verity must catch all errors that could potentially occur. Errors can come from:

- Function calls
    - Implementations should check the signature of the function to figure out what errors should get in the way of the code running.
- Operations
    - Implementations should compare types with operators. Errors should be raised when operating on incompatible types

If a procedure contains an unhandled error, it should return an error and refuse to parse the input. In the case of an interpreter, if it detects that an unhandled error is could possibly arise in the code it is interpreting, it should stop execution with an error. Optionally, this could also be done in a pre-execution parsing pass.

Unchecked errors, if detected, will prevent code from being compiled, and if one slips through the cracks, the program will automatically halt instead of propogating.

If a compiler encoutners redundant error-handling code, it can be removed as part of a compiler optimization.

# 5.0 Standard Library

### 5.1 Why verity needs a well-designed standard library

The standard library must have an `Error` class as aforementioned. The standard library must be convenient to use, as it will be the most commonly used resource by a developer in Verity. Many languages suffer from a miserable standard library, like C++. C++'s standard library has confusingly named classes, with certain data structures implemented differently than their names convey, leading to algorithms with slower-than-intended time efficiencies, because list structures were implemented with trees. Calling into the structure was supposed to have a O(1) time efficiency, but ends up with a O(n log n) time efficiency, slowing down algorithms. C++ also has a notoriously slow std regex parser implementation, and its dependency system includes header files multiple times by default, and requires either header guards or the disputed `#pragma once`. This pattern leads to unneccesary verbosity. 

Verity will use a module system, as found in modern languages, preventing unneccesary complexity.

Additionally, its standard lib will be designed for

- Simplicity
- Consistency
- Accurate/clear naming

### 5.2 Standard lib specification

The standard library must feature a `Math` class/module, with the following members:

- Trigonometry
	- sin
	- cos
	- tan
	- asin
	- acos
	- atan
- Exponentiation
	- exp
	- log
	- pow
	- log10
- Constants
	- e
	- pi
- Randomness
	- randInt
	- randFloat
	- seed

Then, the stdlib must have a `web` module with the following members

- `request` submodule
	- get
	- put
	- post
	- delete
	- `Request` class
	- `Response` class
- `parsing` submodule
	- `jsonparser`
	- `xmlparser`
- `websocket` submodule
	- open
	- close

The parsing submodule may be moved into its own module.
Then, the stdlib must have a `fs` module with the following members:

- `file` submodule
	- read
	- write (also creates)
	- rename
	- append
	- delete
	- `metadata`
		- getSize
		- getLastModified
		- getPermissions
- `folder` submodule
	- list
	- create
	- getPath
	- move
	- rename
	- delete

The standard lib should also include `audio`, `ui`, and `video` utilities, but the details of those modules have not been figured out yet. Additional utilities will also be provided.

A more comprehensive list and description of utilities will be available in a newer version of the specification.

# 6.0 Interoperability, Runtime requirements, & Compilation details

A Verity runtime must follow the implementation details laid out here.

### 6.1 ABI

**This sections only applies to Compilers.** 

Verity will use the C ABI, except higher-level conceps will map to concepts available in the C ABI. It uses C calling conventions.

Classes map to lower level concepts as structures with property variables, with a consistent name mangling scheme.

Class members will be prefixed with their classname, as shown.

- `MyClass.MyMethod()` becomes `member_MyClass_MyMethod`

When downconverting classes, the members of a class become the members of a struct, and procedures are represented in the struct by pointers. Procedures in a class, and in fact, procedures in general, are immutable.

When downconverting dictionaries, its members will be represented with a map.

In C, you can define a pointer to a function with a typedef, and use that to pass a function as a callback. Similarly, methods will be represented as a pointer that points to the procedure's definition.

### 6.2 Imports

In compilers, dependencies can be either statically or dynamically linked, but the source code of a library should not be inserted into the implementations input unless the library was included as a source file by the user. Circuluar imports are allowed.

Implementations should read the contents of an included file to parse its symbols so that it can be separately compiled and linked with the main file.

Imports are declared with the `import` keyword, as shown below. The path is relative to the current file's location, not the CWD.

```
import "std/fs" as fs;
import "./db.vty" as database;
```

Local files, when imported, must be prefixed with the "./" to signal to the compiler that this is a local file, and not a global or project-wide library.

### 6.3 IR Model & Optimization

In the case that you are developing a compiler, you will want to have an IR designed. You can design one yourself, but it is recommended to use an SSA-based IR, and from there, you can optimize the code. The following optimizations are recommended as they are relatively simple to implement:

- Constant folding
- Dead code elimination
	- Elimination of unneccesary `handle` clauses
	- Unused procedure elimination
- Code motion

# 7.0 Grammar

### 7.1 Basic EBNF Grammar

```
program        ::= { import_decl | proc_decl }
import_decl    ::= "import" string_literal "as" identifier ";"
proc_decl      ::= "recursive"? "proc" identifier "(" param_list? ")" return_clause throws_clause? block

param_list     ::= param { "," param }
param          ::= type identifier | "mut" type identifier

return_clause  ::= "returns" type
throws_clause  ::= "throws" type_list
type_list      ::= identifier { "," identifier }
type           ::= identifier
                 | identifier "[" "]"
block          ::= "{" { statement } "}"

statement      ::= var_decl ";"
                 | assignment ";"
                 | return_stmt ";"
                 | throw_stmt ";"
                 | break_stmt ";"
                 | skip_stmt ";"
                 | if_stmt
                 | while_stmt
                 | for_stmt
                 | switch_stmt
                 | expr_stmt ";"
var_decl       ::= ["mut"] type identifier "=" expression
if_stmt        ::= "if" "(" expression ")" block { "else" "if" "(" expression ")" block } [ "else" block ]
while_stmt     ::= "while" "(" expression ")" block
for_stmt       ::= "for" "(" statement expression ";" expression ")" block
expression     ::= logical_or

logical_or     ::= logical_and { "||" logical_and }
logical_and    ::= equality { "&&" equality }
equality       ::= additive { "==" additive }
additive       ::= multiplicative { ("+" | "-") multiplicative }
multiplicative ::= unary { ("*" | "/" | "%") unary }

unary          ::= ("*" | "&" | "-" | "++" | "--") unary
                 | postfix

postfix        ::= primary [ "++" | "--" ]

primary        ::= literal
                 | identifier
                 | "(" expression ")"
                 | call_expr
call_expr      ::= identifier "(" argument_list? ")"
argument_list  ::= expression { "," expression }
expr_stmt      ::= expression [ handle_clause ]

handle_clause  ::= "handle" identifier "=>" identifier "(" ")"
```