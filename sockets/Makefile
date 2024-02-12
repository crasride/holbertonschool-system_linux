# C Compiler
CC = gcc

# Compiler Flags
CFLAGS = -Wall -Werror -Wextra -pedantic -g
TASKS_4 = request.c
TASKS_5 = queries.c
TASKS_6 = headers.c
TASKS_7 = server.c remove_whitespace.c parameters.c
TASKS_8 = root.c status.c remove_whitespace.c create_todo.c
TASKS_9 = root.c status_9.c remove_whitespace.c retrieve_all_todo.c

# Linker Flags (empty in this case)
LDFLAGS =

# Default Target: Build the executable
all: todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5

# Rule to build the executable
todo_api_0: request.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASKS_4) -o todo_api_0

todo_api_1: queries.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASKS_5) -o todo_api_1

todo_api_2: headers.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASKS_6) -o todo_api_2

todo_api_3: parameters.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASKS_7) -o todo_api_3

todo_api_4: create_todo.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASKS_8) -o todo_api_4

todo_api_5: retrieve_all_todo.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(TASKS_8) -o todo_api_5

# Target to clean generated files
clean:
	rm -f todo_api_0 todo_api_1 todo_api_2 todo_api_3 todo_api_4 todo_api_5

# Declaration of phony targets
.PHONY: all clean
