# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -std=c90 -Wall -Werror -Wextra -pedantic -g
TASKS_4 = request.c
TASKS_5 =
TASKS_6 =
TASKS_7 =
TASKS_8 =
TASKS_9 =

# Linker Flags (empty in this case)
LDFLAGS =

# Default Target: Build the executable
all: todo_api_0

# Rule to build the executable
todo_api_0: request.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASKS_4) -o todo_api_0

# Target to clean generated files
clean:
	rm -f todo_api_0

# Declaration of phony targets
.PHONY: all clean
