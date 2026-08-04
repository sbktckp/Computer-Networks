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

From the repo root, `./run compact/3.1` and `./run compact/3.2` do the
same thing.

## What's here

Same server/client pair, shortest form that still compiles clean
under `-Wall -Wextra -Wpedantic` and behaves the same: the server
still loops on `read()` until the client disconnects, the client still
sends 3 messages and prints each reply. What was trimmed:

- No `SO_REUSEADDR` error checking, `perror`/`exit` calls, or usage
  messages, every socket call's return value is used directly without
  a guard.
- No `msg_count` label separated into its own variable name beyond the
  loop counter itself.
- Reply string length is hardcoded (`18`, the length of
  `"Hello from server!"`) instead of computed with `strlen()`.

Nothing here changes the underlying sequence of socket calls, the full
version's comments explain *why* each one is needed; this is the same
logic with the explanation and error handling removed.
