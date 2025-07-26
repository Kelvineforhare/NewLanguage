# Armin

**Armin** is a simple programming language implemented in C++. This project includes a parser and interpreter for the language.

## Features

- C-style syntax with functions, conditionals, loops, and printing
- Basic arithmetic and variable support
- Keywords: `def`, `while`, `if`, `else`, `return`, `print`, `pass`

## Getting Started

### Prerequisites

Ensure you have `make` and a C++ compiler installed (e.g., `g++`).

### Build

To build the interpreter:

```
make
```

### Run

1. Create a file with the `.amn` extension, for example `example.amn`
2. Run the interpreter like so:

```
./armin example.amn
```

## Example Code

Create a file called `test.amn` and paste the following (1 example should exist called test.amn):

```
def fizzBuzz(x){
    while(x > 0){
        if(x % 5 == 0){
            print(x);
        }
        else{
            pass;
        };
        if(x % 3 == 0){
            print(x);
        }
        else{
            pass;
        };
        x = x - 1;
    };
    return 0;
}

x = fizzBuzz(5);
```

## Notes

- `if` always requires an `else`, but you can use `pass` to effectively make it behave like a simple `if`.
- You can add single-line comments in your `.amn` files using `//`.  Anything following `//` on that line will be ignored.


