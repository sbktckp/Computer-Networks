# Compact versions

## Run in your Codespace terminal

Same two-terminal workflow as the full versions, one level up.

**Terminal 1:**

```bash
./run_3.1_tcp_server 9090
```

**Terminal 2:**

```bash
./run_3.2_tcp_client 127.0.0.1 9090
```

Type messages at the `>` prompt, one per line. Type `quit` (or press
Ctrl+D) to end the session. From the repo root, `./run compact/3.1`
and `./run compact/3.2` do the same thing.

## What's here

Same server/client pair, shortest form that still compiles clean
under `-Wall -Wextra -Wpedantic` and behaves the same: the server
still loops on `read()` until `quit` or disconnect, the client still
reads messages from stdin until `quit` or EOF. What was trimmed:

- No `SO_REUSEADDR` error checking, `perror`/`exit` calls, or usage
  messages, every socket call's return value is used directly without
  a guard.
- No separate `message`/`reply` variable naming beyond what the loop
  itself needs.

Nothing here changes the underlying sequence of socket calls or the
interactive read loop, the full version's comments explain *why* each
choice was made; this is the same logic with the explanation and error
handling removed.
