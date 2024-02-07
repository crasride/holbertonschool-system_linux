# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Werror -Wextra -pedantic -g
TASKS_4 = request.c
TASKS_5 = queries.c
TASKS_6 =
TASKS_7 =
TASKS_8 =
TASKS_9 =

# Linker Flags (empty in this case)
LDFLAGS =

# Default Target: Build the executable
all: todo_api_0 todo_api_1

# Rule to build the executable
todo_api_0: request.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASKS_4) -o todo_api_0

todo_api_1: queries.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASKS_5) -o todo_api_1

# Target to clean generated files
clean:
	rm -f todo_api_0 todo_api_1

# Declaration of phony targets
.PHONY: all clean
