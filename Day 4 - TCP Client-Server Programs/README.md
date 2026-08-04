# Day 4: TCP Client-Server Programs

## Run in your Codespace terminal

Every question is a client/server pair, so each needs **two terminal
tabs**: start that question's server first, then run its client
against it.

**Terminal 1:**

```bash
./run_4.4_echo_server 9090
```

**Terminal 2, once the server prints "listening":**

```bash
./run_4.4_echo_client 127.0.0.1 9090
```

Questions 4.3 through 4.6 are interactive: the client prompts with
`>` for as many messages as you want to type, one per line. Type
`quit` (or press Ctrl+D) to end the session cleanly on both sides.
Questions 4.1 and 4.2 need no input at all, the client connects,
prints a line, and exits immediately.

To open a second terminal in Codespaces: click the `+` in the terminal
panel, or `Terminal -> New Terminal` from the menu.

For the compact versions, `cd compact` first and use the same
`run_4.x_*` names. From the repo root, `./run 4.4` (server) and
`./run 4.4` again won't disambiguate server vs client since both share
the `4.4` prefix, so from the root use the full name instead:
`./run 4.4_echo_server` and `./run 4.4_echo_client` (or the
`compact/` equivalents).

Both programs in every pair take arguments:

```bash
./run_4.X_..._server <port>              # default: 8080
./run_4.X_..._client <server_ip> <port>  # defaults: 127.0.0.1 8080
```

## Questions

| No. | Server | Client | What it does |
|-----|--------|--------|---------------|
| 4.1 | `4.1_basic_connection_server.c` | `4.1_basic_connection_client.c` | Accepts one client, both sides print a success message, no data exchanged |
| 4.2 | `4.2_client_info_server.c` | `4.2_client_info_client.c` | Server displays the connecting client's IP and port |
| 4.3 | `4.3_one_way_message_server.c` | `4.3_one_way_message_client.c` | Client sends typed messages; server only reads and displays them, never replies |
| 4.4 | `4.4_echo_server.c` | `4.4_echo_client.c` | Server sends each received message straight back unchanged |
| 4.5 | `4.5_uppercase_server.c` | `4.5_uppercase_client.c` | Server converts the received string to uppercase and sends it back |
| 4.6 | `4.6_reverse_string_server.c` | `4.6_reverse_string_client.c` | Server reverses the received string and sends it back |

## Theory

### Building on Day 3

Day 3 covered the core TCP call sequence, `socket()` -> `bind()` ->
`listen()` -> `accept()` on the server, `socket()` -> `connect()` on
the client, and the byte-order and `struct sockaddr_in` material. This
set of six questions is the same foundation applied progressively:
4.1 proves the connection alone, 4.2 adds reading the client's address
back from `accept()`, and 4.3-4.6 build up the read/reply exchange one
step at a time.

### Why 4.3 needs explicit message framing

TCP is a **byte stream**, not a sequence of discrete messages. When a
program calls `send()` twice in a row, TCP is free to deliver those
bytes to the receiving `read()` as one call, two calls, or even split
across more than two, there is no guarantee that one `send()` lines up
with one `read()`. Questions 4.4-4.6 don't hit this in practice because
each message is followed by a reply that the client waits for before
sending the next one, which naturally paces the traffic. Question 4.3
has no reply to wait for, so two messages typed quickly back to back
can genuinely arrive at the server in a single `read()` call and get
concatenated (`"hello"` immediately followed by `"quit"` can arrive as
`"helloquit"`).

The fix is **message framing**: the client terminates every message
with `\n` before sending it, and the server reads one byte at a time
with a small `read_line()` helper until it sees that `\n`, so it always
extracts exactly one message no matter how TCP happened to batch the
underlying packets. This is the same problem real protocols solve with
a length prefix or a fixed delimiter, HTTP headers end each line with
`\r\n` for exactly this reason.

### The read/transform/send pattern (4.4-4.6)

Questions 4.4, 4.5, and 4.6 share one shape: `read()` a message,
transform it, `send()` the result back. Only the transform step
changes:

- **4.4 (echo)**: the identity transform, the bytes `read()` just
  produced are sent back completely unmodified.
- **4.5 (uppercase)**: each byte is passed through `toupper()` in
  place. Reading each byte as `unsigned char` before the call avoids
  undefined behavior from a negative `char` value on bytes >= 128.
- **4.6 (reverse)**: an in-place two-pointer swap, one index walking
  in from the start of the buffer, one from the end, meeting in the
  middle. This needs exactly `n/2` swaps rather than `n`, since
  swapping past the midpoint would just undo the first half of the
  work.

## Design notes

- **SO_REUSEADDR** is set on every server before `bind()`, so
  restarting a server on the same port immediately after stopping it
  doesn't fail with "Address already in use" while the OS holds the
  port in `TIME_WAIT`.
- **`inet_pton()` over `inet_addr()`** in every client: `inet_addr()`
  returns `INADDR_NONE`, itself a valid bit pattern, on some malformed
  input, so a parsing failure can silently look like a valid address.
  `inet_pton()` has an unambiguous return value.
- **`fgets()` over `scanf("%s")`** in every interactive client:
  `scanf("%s")` stops at the first whitespace, silently truncating any
  multi-word message. `fgets()` reads the whole line.
- **Sessions are driven by client input, not a fixed count.** Every
  server in 4.3-4.6 loops on `read()` until the client sends `quit` or
  disconnects, so a conversation can be one message or a hundred.
