# Compact versions

Same 5 programs, shortest form that still compiles clean under
`-Wall -Wextra -Wpedantic` and produces the same output, and still uses
a loop wherever the full version does. Compact means fewer comments
and shorter names, not different algorithms. What was trimmed:

- **2.1**: no `strtol`-based validation, `atoi` instead (still checks
  `argc`, just not malformed numeric input).
- **2.2**: `dob` field names shortened to `d, m, y`; no separate
  `set_name`/`print_student` helpers, each `printf`/`strcpy` inline.
- **2.3**: still loops over `sizeof(n)`, just without the intermediate
  `bytes[]` array; prints straight from the pointer inside the loop.
- **2.4**: still reassembles via a loop, just folded into the same
  loop that prints each byte instead of two separate passes.
- **2.5**: still uses the union probe and a full reversal loop, just
  without the separate `reverse_bytes` function, inlined into `main`.

Nothing here trims a loop down to a hardcoded expression. The full
version's comments explain *why* each loop bound or type choice was
made; this is the same logic with the explanation removed.
