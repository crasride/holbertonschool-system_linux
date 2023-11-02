# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -std=c90 -Wall -Werror -Wextra -pedantic
FILES_0 = hnm.c

# Linker Flags (empty in this case)
LDFLAGS =

# Default Target: Build the hnm executable
all: hnm

# Rule to build hnm
hnm: $(FILES_0)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Target to clean generated files
clean:
	rm -rf hnm

# Declaration of phony targets
.PHONY: all clean

