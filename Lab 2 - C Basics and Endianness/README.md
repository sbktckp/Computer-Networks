# Lab 2: C Basics and Endianness

## Run in your Codespace terminal

One block per question. Copy, paste, done. Every program here runs
standalone, no server and client pairing needed. Each block `cd`s to
the folder directly, so it's copy-paste safe regardless of where your
terminal currently is, and restores the executable bit before running
(it doesn't survive a fresh clone). These use the standard Codespaces
path, `/workspaces/Computer-Networks`; if you cloned elsewhere, swap
in your own path.

### 2.1 Swap two integers through a pointer

```bash
cd "/workspaces/Computer-Networks/Lab 2 - C Basics and Endianness"
chmod +x run_*
./run_2.1_swap_pointer 10 25
```

### 2.2 Struct call by value against call by address

```bash
cd "/workspaces/Computer-Networks/Lab 2 - C Basics and Endianness"
chmod +x run_*
./run_2.2_struct_call_by_value_address
```

### 2.3 Byte extraction

```bash
cd "/workspaces/Computer-Networks/Lab 2 - C Basics and Endianness"
chmod +x run_*
./run_2.3_byte_extraction 305419896
```

### 2.4 Struct pkt reassembly

```bash
cd "/workspaces/Computer-Networks/Lab 2 - C Basics and Endianness"
chmod +x run_*
./run_2.4_struct_pkt_reassemble 305419896
```

### 2.5 Endianness check

```bash
cd "/workspaces/Computer-Networks/Lab 2 - C Basics and Endianness"
chmod +x run_*
./run_2.5_endianness_check 305419896
```

305419896 is 0x12345678 in decimal, which is the value worth using
because its four bytes are all different, so the byte order is obvious
in the output.

### The compact versions

```bash
cd "/workspaces/Computer-Networks/Lab 2 - C Basics and Endianness/compact"
chmod +x run_*
./run_2.1_swap_pointer 10 25
./run_2.2_struct_call_by_value_address
./run_2.3_byte_extraction 305419896
./run_2.4_struct_pkt_reassemble 305419896
./run_2.5_endianness_check 305419896
```

### From the repo root instead

```bash
cd /workspaces/Computer-Networks
chmod +x run
./run 2.1 10 25
./run compact/2.1 10 25
```

Each wrapper compiles its matching `.c` file with
`-std=c17 -O2 -Wall -Wextra -Wpedantic` and runs it immediately, so an
edit to the source is picked up on the next run with no separate build
step.

## Theory

### Socket programming

A socket is the endpoint of a two-way communication link between two
programs on a network, one process on the local host, another
possibly on a remote one, identified by an IP address and a port
number. Socket programming in C is built on a small set of system
calls: `socket()` creates the endpoint, `bind()` attaches it to an
address, `listen()`/`accept()` set up a server to wait for and admit
connections, `connect()` lets a client reach a listening server, and
`send()`/`recv()` (or plain `read()`/`write()`) move bytes once the
connection exists. Everything this assignment covers, pointers,
structs, and especially byte layout, is the groundwork socket code
depends on: a `struct sockaddr_in` is a plain C struct passed by
address into `bind()`/`connect()`, and any multi-byte field inside a
network packet (port numbers, IP addresses, packet headers) has to be
placed in a byte order both ends agree on, which is exactly what
question 2.5 is about.

### Endianness

Endianness is the order in which a multi-byte value's bytes are stored
in memory. Little-endian machines (x86, most of today's hardware) store
the least significant byte at the lowest address; big-endian machines
store the most significant byte first. A number like `0x12345678`
therefore sits in memory as `78 56 34 12` on a little-endian host and
`12 34 56 78` on a big-endian one: same value, different byte layout.

This matters on a network because two machines with opposite
endianness can misinterpret each other's multi-byte fields if nobody
normalizes them. The standard fix is network byte order: TCP/IP
mandates big-endian for header fields, and C's `htons()`/`htonl()`
convert a value from host order to network order before it goes on the
wire, with `ntohs()`/`ntohl()` reversing the conversion on receipt. The
byte-swap in 2.5 is the same operation those functions perform for a
32-bit word.

## Questions

| No. | File | What it does |
|-----|------|---------------|
| 2.1 | `2.1_swap_pointer.c` | Swap two `argv`-supplied ints via a function taking `int*`, with real input validation via `strtol` |
| 2.2 | `2.2_struct_call_by_value_address.c` | Nested struct (`student_info` holds `dob`); one function mutates by value (invisible to caller), one by address (visible); `strncpy` with guaranteed termination |
| 2.3 | `2.3_byte_extraction.c` | Extract every byte of an `int` in a loop over `sizeof(int)`, not hardcoded to 4 bytes |
| 2.4 | `2.4_struct_pkt_reassemble.c` | Split a number across `struct pkt {char; char[2]; char}` by direct member assignment (the struct shape is fixed by the question), then reassemble via a loop that folds the bytes back |
| 2.5 | `2.5_endianness_check.c` | Union-based endianness probe (one comparison, no loop needed), then a generic in-place byte-reversal loop that swaps endianness for any integer width |

## Design philosophy

Each program uses a loop exactly where iteration is the right model
for the problem, and a direct expression exactly where it isn't:

- **2.1**: two variables, nothing to iterate over. The engineering
  effort goes into rejecting malformed input via `strtol`, not into
  avoiding a loop that was never needed.
- **2.2**: struct member count is fixed by the type definition, so
  population and printing are direct member access. `strncpy` with an
  explicit terminator replaces raw `strcpy` to avoid an overflow if a
  longer name were substituted in.
- **2.3**: "extract every byte of a value" is inherently a bounded
  iteration over `sizeof(int)`, the loop is the natural fit here, and
  using `sizeof` instead of a literal `4` means the code is still
  correct if compiled on a platform with a different `int` width.
- **2.4**: filling the struct is 4 direct assignments (C has no
  reflection over struct members, so there's nothing to loop over
  there), but reassembling the bytes back into a number is written as
  a loop over an array of those same bytes, the version that actually
  scales if the packet grew another field.
- **2.5**: detecting endianness is a single comparison against a
  union's first byte, so it's one line, not a loop. Reversing the byte
  order, on the other hand, is naturally a loop that swaps from both
  ends toward the middle, which works for any integer width rather
  than a fixed 4-byte bitmask expression.

## Complexity

| No. | Time | Space |
|-----|------|-------|
| 2.1 | O(1) | O(1) |
| 2.2 | O(1) | O(1) |
| 2.3 | O(sizeof(int)) = O(1) for a fixed-width int | O(sizeof(int)) |
| 2.4 | O(1) fill, O(4) reassembly | O(1) |
| 2.5 | O(1) detect, O(sizeof(int)/2) swap | O(1) |
