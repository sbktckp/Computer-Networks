#!/usr/bin/env bash
# Builds every program in every top-level week folder ("Assignment N - ..."
# or "Day N - ..."). Used by `make all` and by ./run; not meant to be
# invoked directly, though it works fine standalone too.
set -euo pipefail
cd "$(dirname "$0")"

CC=gcc
CFLAGS="-std=c17 -O2 -Wall -Wextra -Wpedantic"

shopt -s nullglob
for day in "Assignment"* "Day"*; do
    [ -d "$day" ] || continue
    mkdir -p "bin/$day" "bin/$day/compact"
    for f in "$day"/*.c; do
        name="$(basename "$f" .c)"
        $CC $CFLAGS -o "bin/$day/$name" "$f" -lm
    done
    for f in "$day"/compact/*.c; do
        name="$(basename "$f" .c)"
        $CC $CFLAGS -o "bin/$day/compact/$name" "$f" -lm
    done
done
