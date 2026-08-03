# Computer Networks Lab

Weekly codes for the Computer Networks lab. Same layout as the DAA repo:
each week gets a folder with a full version of every program and a
`compact/` subfolder with the shortest version that still works.

## Running

Open Codespaces on this repo. The container installs `gcc` and `make`
before it hands you a terminal.

**Open a `.c` file and press Run**, the triangle at the top right of the
editor. Compiles and runs that one file.

**Or use the terminal:**

```bash
./run              # list everything
./run 2.1          # question 2.1, full version
./run compact/2.1  # question 2.1, compact version
./run 2.3 999999   # extra words are forwarded as argv to the program
```

`make` rebuilds everything, `make clean` removes the binaries. No
absolute paths anywhere, standard C17 only, so it behaves the same in a
Codespace, WSL, Linux, or macOS.

## Weeks

| Week | Folder | Topic |
|------|--------|-------|
| 2 | [Assignment 2 - C Basics and Endianness](<Assignment 2 - C Basics and Endianness>) | Pointers, structs, byte extraction, endianness |

## Full vs Compact

**Full** versions are commented for understanding: they explain *why*
each trick works (call-by-value copying the whole struct, unsigned char
avoiding sign extension, union-based endianness probes), not just *what*
the code does.

**Compact** versions are the shortest code that still compiles clean
under `-Wall -Wextra -Wpedantic` and produces the same output — fixed
arrays, no loops where a direct unrolled expression does the job, no
error handling beyond argument count. Each compact file's header comment
carries a real sample run so the program and its output can go straight
into a record book.
