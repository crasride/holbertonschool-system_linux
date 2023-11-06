# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -std=c90 -Wall -Werror -Wextra -pedantic
FILES_0 = hnm_main.c hnm_64.c hnm_32_big_endian.c hnm_32_little_endian.c hnm_32_solaris.c

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

