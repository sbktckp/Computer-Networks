# Computer Networks Lab

Weekly codes for the Computer Networks lab. Each week gets a folder with
a full version of every program and a `compact/` subfolder with the
shortest version that still works. Every program is directly
executable in the terminal, no manual `gcc` command needed.

## Run in your Codespace terminal

Open this repo in GitHub Codespaces (Code button -> Codespaces -> Create
codespace on main). The devcontainer installs `gcc`, `make`, and the
C/C++ extension automatically, so the terminal is ready as soon as the
codespace loads. No setup step is needed.

Once the terminal is open, there are three ways to run any program:

**1. Run a program directly** (fastest, one command per question):

```bash
cd "Assignment 2 - C Basics and Endianness"
./run_2.1_swap_pointer 10 25
./run_2.3_byte_extraction 999999
cd compact
./run_2.5_endianness_check 999999
```

Every `run_*` script is already executable and compiles fresh each time
it runs, so editing the `.c` file and re-running picks up the change
immediately.

**2. Use the repo-wide `./run` helper** (works from the repo root):

```bash
./run                # list every program in the repo
./run 2.1             # full version of question 2.1
./run compact/2.1     # compact version of question 2.1
./run 2.3 999999       # extra words are forwarded as argv
```

**3. Use `make`** (builds every program at once into `bin/`):

```bash
make            # builds bin/2.x_*.c and bin/compact/2.x_*.c
./bin/2.1_swap_pointer 10 25
make clean      # removes bin/
```

**Or click Run** in the editor: open any `.c` file, press the triangle
at the top right. This compiles and runs just that file.

If a script ever loses its executable bit (some file managers strip it
on download), restore it with:

```bash
chmod +x run "Assignment 2 - C Basics and Endianness"/run_* \
             "Assignment 2 - C Basics and Endianness"/compact/run_*
```

No absolute paths anywhere in the repo, and everything targets standard
C17, so all three methods behave identically in a Codespace, WSL,
native Linux, or macOS.

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
under `-Wall -Wextra -Wpedantic` and produces the same output, using a
loop wherever the full version does. Each compact file's header comment
carries a real sample run, and each has its own `run_*` wrapper, so it
stands on its own without needing the full version for context.
