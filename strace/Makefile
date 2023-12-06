# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Werror -Wextra -pedantic
FILES_0 = strace_0.c
FILES_1 = strace_1.c


# Linker Flags (empty in this case)
LDFLAGS =

# Default Target: Build the executable
all: strace_0 strace_1

# Rule to build 0-hreadelf from 0-hreadelf.c
strace_0: strace_0.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

strace_1: strace_1.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# Target to clean generated files
clean:
	rm -f strace_0 strace_1
# Declaration of phony targets
.PHONY: all clean
