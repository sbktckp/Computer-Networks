CC      := gcc
CFLAGS  := -std=c17 -O2 -Wall -Wextra -Wpedantic
LDLIBS  := -lm
DAY     := Assignment 2 - C Basics and Endianness

all:
	mkdir -p bin bin/compact
	@for f in "$(DAY)"/*.c; do \
		name=$$(basename "$$f" .c); \
		$(CC) $(CFLAGS) -o "bin/$$name" "$$f" $(LDLIBS) || exit 1; \
	done
	@for f in "$(DAY)"/compact/*.c; do \
		name=$$(basename "$$f" .c); \
		$(CC) $(CFLAGS) -o "bin/compact/$$name" "$$f" $(LDLIBS) || exit 1; \
	done

clean:
	rm -rf bin

.PHONY: all clean
