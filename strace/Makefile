# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -std=c90 -Wall -Werror -Wextra -pedantic
FILES_0 = strace_0.c
FILES_1 = strace_1.c
FILES_2 = strace_2.c
FILES_3 = strace_3.c


# Linker Flags (empty in this case)
LDFLAGS =

# Default Target: Build the executable
all: strace_0 strace_1 strace_2 strace_3

# Rule to build the executable
strace_0: strace_0.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

strace_1: strace_1.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

strace_2: strace_2.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

strace_3: strace_3.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# Target to clean generated files
clean:
	rm -f strace_0 strace_1 strace_2 strace_3
# Declaration of phony targets
.PHONY: all clean
