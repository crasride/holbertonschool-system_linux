# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Werror -Wextra -pedantic
FILES_0 = 0-hreadelf_tools.c

# Linker Flags (empty in this case)
LDFLAGS =

# Default Target: Build the 0-hreadelf executable
all: 0-hreadelf

# Rule to build 0-hreadelf from 0-hreadelf.c
0-hreadelf: 0-hreadelf.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(FILES_0) -o $@ $<

# Target to clean generated files
clean:
	rm -f 0-hreadelf

# Declaration of phony targets
.PHONY: all clean
