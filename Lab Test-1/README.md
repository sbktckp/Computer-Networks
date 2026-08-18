# Lab Test-1: TCP Socket Programming

## Run in your Codespace terminal

Both tasks are client/server pairs, so each needs **two terminal tabs**:
start that task's server first, then run its client against it. To
open a second tab, click the `+` in the terminal panel, or use
`Terminal -> New Terminal`.

Each task uses its own port, 9191 and 9192, so both pairs can run at
once in different tabs without colliding.

### Task-1: TCP Calculator

Terminal 1:

```bash
cd "Lab Test-1"
./run_1.1_calculator_server 9191
```

Terminal 2:

```bash
cd "Lab Test-1"
./run_1.1_calculator_client 127.0.0.1 9191
```

Enter two integers and an operator (`+`, `-`, `*`, `/`) at the prompts.
The server performs the operation, prints the operands and result, and
sends the result back to the client. Dividing by zero returns
`Error: division by zero` instead of crashing.

### Task-2: TCP Word Count

Terminal 1:

```bash
cd "Lab Test-1"
./run_2.1_word_count_server 9192
```

Terminal 2:

```bash
cd "Lab Test-1"
./run_2.1_word_count_client 127.0.0.1 9192
```

Type a sentence at the prompt. The server counts the words (handling
multiple spaces and leading/trailing spaces correctly) and sends the
count back to the client.

## Full vs Compact

**Full** versions are commented for understanding: they explain *why*
each choice works, not just *what* the code does.

**Compact** versions in `compact/` are the shortest code that still
compiles clean under `-Wall -Wextra -Wpedantic` and produces the same
output. Each has its own `run_*` wrapper, so it stands on its own.
