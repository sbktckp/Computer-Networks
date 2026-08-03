# Assignment 2: C Basics and Endianness

## Theory

### Socket programming

A socket is the endpoint of a two-way communication link between two
programs on a network — one process on the local host, another
possibly on a remote one, identified by an IP address and a port
number. Socket programming in C is built on a small set of system
calls: `socket()` creates the endpoint, `bind()` attaches it to an
address, `listen()`/`accept()` set up a server to wait for and admit
connections, `connect()` lets a client reach a listening server, and
`send()`/`recv()` (or plain `read()`/`write()`) move bytes once the
connection exists. Everything this assignment covers — pointers,
structs, and especially byte layout — is the groundwork socket code
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
`12 34 56 78` on a big-endian one — same value, different byte layout.

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
| 2.1 | `2.1_swap_pointer.c` | Swap two `argv`-supplied ints via a function taking `int*` |
| 2.2 | `2.2_struct_call_by_value_address.c` | Nested struct (`student_info` holds `dob`); one function mutates by value (invisible to caller), one by address (visible) |
| 2.3 | `2.3_byte_extraction.c` | Reinterpret an `int`'s address as `unsigned char*`, read its 4 bytes into named variables, no loop |
| 2.4 | `2.4_struct_pkt_reassemble.c` | Split a number across `struct pkt {char; char[2]; char}`, then reaggregate and confirm it matches |
| 2.5 | `2.5_endianness_check.c` | Union-based endianness probe, print byte layout, byte-swap to the opposite endianness |

## Complexity

All five questions operate on a single fixed-size value (an `int`, a
struct with a handful of members). Every operation is O(1) time and
O(1) space — there is nothing here that scales with an input size, so
the usual complexity table is a formality: no loops, no recursion, no
allocation, just fixed arithmetic and pointer/struct member access.
