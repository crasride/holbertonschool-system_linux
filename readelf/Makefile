# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -std=c90 -Wall -Werror -Wextra -pedantic
FILES_0 = 0-hreadelf_tools.c 0-hreadelf_convert.c
FILES_1 = 1-hreadelf_tools.c 0-hreadelf_convert.c 1-hreadelf_print.c
FILES_2 = 2-hreadelf_tools.c 0-hreadelf_convert.c 2-hreadelf_print.c 2-hreadelf_get_type.c 2-hreadelf_mapping64.c 2-hreadelf_mapping32_1.c 2-hreadelf_mapping32_2.c 2-hreadelf_check.c

# Linker Flags (empty in this case)
LDFLAGS =

# Default Target: Build the 0-hreadelf executable
all: 0-hreadelf 1-hreadelf 2-hreadelf

# Rule to build 0-hreadelf from 0-hreadelf.c
0-hreadelf: 0-hreadelf.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(FILES_0) -o $@ $<
1-hreadelf: 1-hreadelf.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(FILES_1) -o $@ $<
2-hreadelf: 2-hreadelf.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(FILES_2) -o $@ $<


# Target to clean generated files
clean:
	rm -f 0-hreadelf 1-hreadelf 2-hreadelf

# Declaration of phony targets
.PHONY: all clean
