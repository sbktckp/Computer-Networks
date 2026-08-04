# Day 3: Socket Programming in C

## Run in your Codespace terminal

TCP needs a server listening before a client can connect, so this
needs **two terminal tabs** open at the same time in your Codespace.

**Terminal 1, start the server first:**

```bash
./run_3.1_tcp_server 9090
```

It prints `Server listening on port 9090...` and then blocks, waiting
for a connection. Leave this terminal running.

**Terminal 2, run the client against it:**

```bash
./run_3.2_tcp_client 127.0.0.1 9090
```

The client connects, sends 3 messages, prints each server reply, then
exits. Terminal 1 will print each message it received and
`Client disconnected.` once the client closes the connection; the
server then exits too, since this server handles one client per run.

To open a second terminal in Codespaces: click the `+` in the terminal
panel, or `Terminal -> New Terminal` from the menu.

For the compact versions, `cd compact` first and use the same
`run_3.1_tcp_server` / `run_3.2_tcp_client` names. From the repo root,
`./run 3.1` and `./run 3.2` (or `./run compact/3.1`, `./run compact/3.2`)
do the same thing, but still need two terminals since the server blocks.

Both programs take arguments:

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
Assignment 2. `htons()` converts a port number from host to network
byte order before it goes into `sin_port`; `ntohs()` converts back when
reading a port out of a received address, which is why the server uses
`ntohs(client_addr.sin_port)` when printing the client's port.

### The TCP call sequence

Server: `socket()` to create the endpoint, `bind()` to attach it to a
port, `listen()` to start queuing incoming connections, `accept()` to
pull one off the queue and get a new socket dedicated to that client,
then `read()`/`send()` in a loop until the client disconnects.

Client: `socket()`, then `connect()` to reach the server (this performs
the TCP three-way handshake), then `send()`/`read()` for as many
exchanges as needed.

`inet_pton()` converts a human-readable IP string ("127.0.0.1") into
the binary form the struct needs; `inet_ntop()` does the reverse, used
here so the server can print the connecting client's IP as text.

## Files

| No. | File | What it does |
|-----|------|---------------|
| 3.1 | `3.1_tcp_server.c` | Accepts one TCP client, loops reading and echoing messages until the client disconnects |
| 3.2 | `3.2_tcp_client.c` | Connects to a TCP server, sends 3 messages in a loop, prints each reply |

## Design notes

- **SO_REUSEADDR** is set on the server socket before `bind()`. Without
  it, restarting the server right after stopping it fails with
  "Address already in use", because the OS holds the port in
  `TIME_WAIT` for a short period after close.
- **The server loops on `read()`** instead of handling a single
  message. A TCP connection is a stream, not a single request/reply,
  so a real server keeps serving the same client until `read()` returns
  0 (clean disconnect) or a negative value (error), which is exactly
  the condition this loop checks.
- **`inet_pton()` over `inet_addr()`** in the client: `inet_addr()`
  returns `INADDR_NONE`, itself a valid bit pattern, on some malformed
  input, so a parsing failure can silently look like a valid address.
  `inet_pton()` has an unambiguous return value and works for both
  IPv4 and IPv6.
- **Command-line port and IP** (with sane defaults) mean the same
  compiled binary can be pointed at a different port or host without
  a rebuild, useful once this same client is tested against a server
  running on another machine.
