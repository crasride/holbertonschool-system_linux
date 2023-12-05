# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -std=c90 -Wall -Werror -Wextra -pedantic
FILES_0 = strace_0.c


# Linker Flags (empty in this case)
LDFLAGS =

# Default Target: Build the executable
all: strace_0

# Rule to build 0-hreadelf from 0-hreadelf.c
strace_0: strace_0.c

	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<



# Target to clean generated files
clean:
	rm -f strace_0
# Declaration of phony targets
.PHONY: all clean
