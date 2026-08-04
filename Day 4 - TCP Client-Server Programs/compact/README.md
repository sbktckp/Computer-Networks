# Compact versions

## Run in your Codespace terminal

Same two-terminal, one-server-one-client-per-question workflow as the
full versions, one level up.

**Terminal 1:**

```bash
./run_4.4_echo_server 9090
```

**Terminal 2:**

```bash
./run_4.4_echo_client 127.0.0.1 9090
```

For questions 4.3-4.6, type messages at the `>` prompt, one per line;
type `quit` (or press Ctrl+D) to end. Questions 4.1 and 4.2 need no
input, the client connects and exits on its own.

## What's here

Same 6 server/client pairs, shortest form that still compiles clean
under `-Wall -Wextra -Wpedantic` and behaves the same, including 4.3's
newline-delimited framing (`read_line()` reading byte-by-byte until
`\n`), which is not something that could safely be trimmed away
without reintroducing the message-concatenation bug the full version's
comments explain. What was trimmed instead:

- No `argc` usage-message checks, `perror`/`exit` calls, every socket
  call's return value is used directly without a guard.
- No separate helper functions beyond `read_line()` (4.3) and
  `reverse_bytes()` (4.6), which stay since removing them would mean
  rewriting the same logic inline twice (once per direction) instead
  of once.

Nothing here changes the underlying sequence of socket calls or the
message framing, the full version's comments explain *why* each choice
was made; this is the same logic with the explanation and error
handling removed.
