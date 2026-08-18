# Lab 3: Socket Programming in C

## Run in your Codespace terminal

TCP needs a server listening before a client can connect, so this needs
**two terminal tabs** open at the same time. To open a second one, click
the `+` in the terminal panel, or use `Terminal -> New Terminal`.

### 3.1 TCP server, run this first

Terminal 1:

```bash
cd "Lab 3 - Socket Programming"
./run_3.1_tcp_server 9090
```

It prints `Server listening on port 9090...` and then blocks, waiting
for a connection. Leave this terminal running.

### 3.2 TCP client, run this second

Terminal 2:

```bash
cd "Lab 3 - Socket Programming"
./run_3.2_tcp_client 127.0.0.1 9090
```

The client connects and prompts with `>` for as many messages as you
want to type. Type a message, press Enter, and the server's reply prints
right after. Type `quit` or press Ctrl+D to end the session, and both
sides exit cleanly.

### The compact versions

Terminal 1:

```bash
cd "Lab 3 - Socket Programming/compact"
./run_3.1_tcp_server 9090
```

Terminal 2:

```bash
cd "Lab 3 - Socket Programming/compact"
./run_3.2_tcp_client 127.0.0.1 9090
```

### From the repo root instead

```bash
./run 3.1 9090
./run 3.2 127.0.0.1 9090
```

Still two terminals, since the server blocks.

### If you get permission denied

The executable bit does not survive a fresh clone, so set it once:

```bash
cd /workspaces/Computer-Networks
find . -name 'run_*' -exec chmod +x {} \;
chmod +x run build.sh
```

### If the port is already in use

Pick a different one. Both programs take the port as an argument, so
9091, 9092 and so on all work, as long as the server and client agree:

```bash
./run_3.1_tcp_server <port>              # default: 8080
./run_3.2_tcp_client <server_ip> <port>  # defaults: 127.0.0.1 8080
```

## Theory

### Sockets

A socket is a software endpoint for communication between two
processes, on the same machine or across a network. It sits between
the application and the transport layer, identified by an IP address,
a port number, and a protocol (TCP or UDP). Stream sockets (`SOCK_STREAM`,
used with TCP) give reliable, ordered, connection-oriented delivery.
Datagram sockets (`SOCK_DGRAM`, used with UDP) are connectionless and
faster, with no delivery or ordering guarantee, suited to real-time
traffic like video or gaming where a late packet is worse than a lost one.

### Key structures

`struct sockaddr` is the generic address type every socket function
signature expects (`bind`, `connect`, `accept`), so the API is not
tied to one address family. `struct sockaddr_in` is the IPv4-specific
version actually used to fill in an address: it holds the address
family, port (`sin_port`), and IP (`sin_addr`, itself a `struct in_addr`
wrapping the 32-bit address). Because `bind`/`connect`/`accept` expect
`struct sockaddr *`, a `struct sockaddr_in *` is always cast to that
type when passed in, this program does that at every call site.

### Byte order matters here too

`sin_port` and `sin_addr` are stored in network byte order (big-endian),
regardless of the host's own endianness, the same concept covered in
Lab 2. `htons()` converts a port number from host to network
byte order before it goes into `sin_port`; `ntohs()` converts back when
reading a port out of a received address, which is why the server uses
`ntohs(client_addr.sin_port)` when printing the client's port.

### The TCP call sequence

Server: `socket()` to create the endpoint, `bind()` to attach it to a
port, `listen()` to start queuing incoming connections, `accept()` to
pull one off the queue and get a new socket dedicated to that client,
then `read()`/`send()` in a loop for as long as the client keeps
sending messages.

Client: `socket()`, then `connect()` to reach the server (this performs
the TCP three-way handshake), then `send()`/`read()` once per message
the user types.

`inet_pton()` converts a human-readable IP string ("127.0.0.1") into
the binary form the struct needs; `inet_ntop()` does the reverse, used
here so the server can print the connecting client's IP as text.

## Files

| No. | File | What it does |
|-----|------|---------------|
| 3.1 | `3.1_tcp_server.c` | Accepts one TCP client, exchanges messages with it until the client sends `quit` or disconnects |
| 3.2 | `3.2_tcp_client.c` | Connects to a TCP server, reads messages from the keyboard and sends each one, until `quit` or EOF (Ctrl+D) |

## Design notes

- **SO_REUSEADDR** is set on the server socket before `bind()`. Without
  it, restarting the server right after stopping it fails with
  "Address already in use", because the OS holds the port in
  `TIME_WAIT` for a short period after close.
- **The session is driven by the client's input, not a fixed count.**
  The server loops on `read()` and the client loops on `fgets()` from
  stdin, so a conversation can be one message or a hundred; either side
  ending it (client typing `quit`, or the client sending EOF) stops the
  loop cleanly on both ends.
- **`fgets()` over `scanf("%s")`** in the client: `scanf("%s")` stops
  at the first whitespace, so a multi-word message would be silently
  truncated. `fgets()` reads the whole line up to the buffer size.
- **The trailing newline `fgets()` keeps is stripped** with
  `strcspn(message, "\n")` before sending, so the server's
  `strcmp(buffer, "quit")` check matches "quit" exactly rather than
  failing to match "quit\n".
- **`inet_pton()` over `inet_addr()`** in the client: `inet_addr()`
  returns `INADDR_NONE`, itself a valid bit pattern, on some malformed
  input, so a parsing failure can silently look like a valid address.
  `inet_pton()` has an unambiguous return value and works for both
  IPv4 and IPv6.
- **Command-line port and IP** (with sane defaults) mean the same
  compiled binary can be pointed at a different port or host without
  a rebuild, useful once this same client is tested against a server
  running on another machine.
