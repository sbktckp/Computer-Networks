# Computer Networks Lab

Weekly codes for the Computer Networks lab. Each week gets its own
folder with a full version of every program and a `compact/`
subfolder with the shortest version that still works. Every program
is directly executable in the terminal, no manual `gcc` command needed.

## Run in your Codespace terminal

Open this repo in GitHub Codespaces (Code button -> Codespaces -> Create
codespace on main). The devcontainer installs `gcc`, `make`, and the
C/C++ extension automatically, so the terminal is ready as soon as the
codespace loads. No setup step is needed.

Once the terminal is open, there are three ways to run any program:

**1. Run a program directly** (fastest, one command per question):

Every block below jumps to the repo root first, so it's copy-paste
safe regardless of where your terminal currently is, and restores the
executable bit before running (it doesn't survive a fresh clone).

```bash
cd "$(git rev-parse --show-toplevel)/Lab 2 - C Basics and Endianness"
chmod +x run_*
./run_2.1_swap_pointer 10 25
```

```bash
cd "$(git rev-parse --show-toplevel)/Lab 3 - Socket Programming"
chmod +x run_*
./run_3.1_tcp_server 9090       # start this first, in its own terminal
```
```bash
cd "$(git rev-parse --show-toplevel)/Lab 3 - Socket Programming"
chmod +x run_*
./run_3.2_tcp_client 127.0.0.1 9090   # then this, in a second terminal
```

```bash
cd "$(git rev-parse --show-toplevel)/Lab 4 - TCP Client-Server Programs"
chmod +x run_*
./run_4.4_echo_server 9090      # start this first, in its own terminal
```
```bash
cd "$(git rev-parse --show-toplevel)/Lab 4 - TCP Client-Server Programs"
chmod +x run_*
./run_4.4_echo_client 127.0.0.1 9090  # then this, in a second terminal
```

```bash
cd "$(git rev-parse --show-toplevel)/Lab 5 - TCP Chat Application"
chmod +x run_*
./run_5.1_chat_server           # start this first, in its own terminal
```
```bash
cd "$(git rev-parse --show-toplevel)/Lab 5 - TCP Chat Application"
chmod +x run_*
./run_5.1_chat_client            # then this, in a second terminal
```

```bash
cd "$(git rev-parse --show-toplevel)/Lab 6 - Concurrent Chat Server"
chmod +x run_*
./run_6.1_concurrent_chat_server         # start this first, in its own terminal
```
```bash
cd "$(git rev-parse --show-toplevel)/Lab 6 - Concurrent Chat Server"
chmod +x run_*
./run_6.1_concurrent_chat_client          # then this, in a second (or third...) terminal
```

```bash
cd "$(git rev-parse --show-toplevel)/Lab Test-1"
chmod +x run_*
./run_1.1_calculator_server 9191      # start this first, in its own terminal
```
```bash
cd "$(git rev-parse --show-toplevel)/Lab Test-1"
chmod +x run_*
./run_1.1_calculator_client 127.0.0.1 9191  # then this, in a second terminal
```

Client/server programs like Lab 3, Lab 4, Lab 5, Lab 6, and Lab Test-1
need two terminal tabs open at once, one running the server, one
running the client against it (Lab 6 can take three or more, to show
several clients chatting at once). See that week's README for the
exact walkthrough.

Every `run_*` script compiles fresh each time it runs, so editing the
`.c` file and re-running picks up the change immediately.

**2. Use the repo-wide `./run` helper** (works from the repo root):

```bash
cd "$(git rev-parse --show-toplevel)"
chmod +x run
./run                      # list every program in the repo, grouped by week
./run 2.1                   # Lab 2, full version of question 2.1
./run compact/2.1           # Lab 2, compact version of question 2.1
./run 3.1                    # Lab 3, the server
./run 3.2 127.0.0.1 9090        # Lab 3, the client, with arguments forwarded
./run 4.4_echo_server            # Lab 4, question 4.4's server (server/client share
./run 4.4_echo_client            # a numeric prefix, so use the full name to pick one)
./run 5.1_chat_server                # Lab 5, chat server
./run 5.1_chat_client                # Lab 5, chat client
./run 6.1_concurrent_chat_server         # Lab 6, concurrent server
./run 6.1_concurrent_chat_client         # Lab 6, chat client (run several at once)
./run 1.1_calculator_server         # Lab Test-1, calculator server
./run 1.1_calculator_client         # Lab Test-1, calculator client
```

**3. Use `make`** (builds every program in every week at once into `bin/`):

```bash
cd "$(git rev-parse --show-toplevel)"
make            # builds bin/<week>/<program> for every week folder
./bin/"Lab 2 - C Basics and Endianness"/2.1_swap_pointer 10 25
make clean      # removes bin/
```

**Or click Run** in the editor: open any `.c` file, press the triangle
at the top right. This compiles and runs just that file.

No absolute paths anywhere in the repo, and everything targets standard
C17, so all three methods behave identically in a Codespace, WSL,
native Linux, or macOS.

## Weeks

| Week | Folder | Topic |
|------|--------|-------|
| 2 | [Lab 2 - C Basics and Endianness](<Lab 2 - C Basics and Endianness>) | Pointers, structs, byte extraction, endianness |
| 3 | [Lab 3 - Socket Programming](<Lab 3 - Socket Programming>) | TCP sockets in C: server and client, interactive session |
| 4 | [Lab 4 - TCP Client-Server Programs](<Lab 4 - TCP Client-Server Programs>) | Six client/server pairs: connection basics, client info, one-way messaging, echo, uppercase, reverse |
| 5 | [Lab 5 - TCP Chat Application](<Lab 5 - TCP Chat Application>) | Interactive two-way TCP chat, ends on "logout" from either side |
| 6 | [Lab 6 - Concurrent Chat Server](<Lab 6 - Concurrent Chat Server>) | fork()-based concurrent chat server: one child process per client, per-client "logout", client IP printed with each message |
| Lab Test 1 | [Lab Test-1](<Lab Test-1>) | TCP calculator (+, -, *, /, division-by-zero handling), TCP word count |

## Full vs Compact

**Full** versions are commented for understanding: they explain *why*
each choice works (call-by-value copying the whole struct, `SO_REUSEADDR`
avoiding a bind failure on restart, `inet_pton` over `inet_addr`), not
just *what* the code does.

**Compact** versions are the shortest code that still compiles clean
under `-Wall -Wextra -Wpedantic` and produces the same output, using a
loop wherever the full version does. Each compact file's header comment
carries a real sample run, and each has its own `run_*` wrapper, so it
stands on its own without needing the full version for context.
