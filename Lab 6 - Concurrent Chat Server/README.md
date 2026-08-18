# Lab 6: Concurrent Chat Server

## Run in your Codespace terminal

This server accepts multiple clients at once, so it makes most sense
with **three or more terminal tabs**: one for the server, and one for
each client you want to connect simultaneously. To open a new tab,
click the `+` in the terminal panel, or use `Terminal -> New Terminal`.

Terminal 1:

```bash
cd "Lab 6 - Concurrent Chat Server"
./run_6.1_concurrent_chat_server
```

Terminal 2:

```bash
cd "Lab 6 - Concurrent Chat Server"
./run_6.1_concurrent_chat_client
```

Terminal 3 (optional, to see the concurrency):

```bash
cd "Lab 6 - Concurrent Chat Server"
./run_6.1_concurrent_chat_client
```

Each client that connects gets its own child process on the server,
forked right after `accept()`. The server prints every client's IP
address alongside its messages, e.g. `[127.0.0.1] Client: hello`, so
you can tell which connection a line belongs to when several are
chatting at once. Typing `logout` in one client's terminal ends only
that client's connection and that client's child process; every other
connected client keeps chatting undisturbed.

## Full vs Compact

**Full** version is commented for understanding: it explains *why*
each choice works (parent closing its copy of the accepted socket,
child closing its copy of the listening socket, `SIGCHLD` set to
`SIG_IGN` so finished children don't linger as zombies), not just
*what* the code does.

**Compact** version in `compact/` is the shortest code that still
compiles clean under `-Wall -Wextra -Wpedantic` and produces the same
output. It has its own `run_*` wrapper, so it stands on its own.
