#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "_getline.h"

#ifndef READ_SIZE
#define READ_SIZE 1024
#endif


/**
 * _getline - Reads a line from a file descriptor and returns it as a string.
 * @fd: The file descriptor from which to read the line.
 * Return: On success, it returns a pointer to the line as a string.
 * If fd is -1, it frees the resources and returns NULL.
 * If the end-of-file is reached or an error occurs, it returns NULL.
 */
char *_getline(const int fd)
{
	static line_head *lines;
	line_head *current_node;
	char *read_data_buffer;
	int bytes_read;

	if (fd == -1)
	{
		free_lines(lines);
		return (NULL);
	}

	for (current_node = lines; current_node != NULL;
		current_node = current_node->next)
	{
		if (current_node->fd == fd)
		{
			if (current_node->bytes <= 0)
				current_node->bytes = read(fd, current_node->buffer, READ_SIZE);
			return (read_line_chars(current_node));
		}
	}

	read_data_buffer = malloc(sizeof(char) * READ_SIZE);
	bytes_read = read(fd, read_data_buffer, READ_SIZE);
	if (bytes_read <= 0)
	{
		free(read_data_buffer);
		return (NULL);
	}

	current_node = add_line_node(&lines, fd, read_data_buffer, bytes_read);
	if (!current_node)
	{
		free(read_data_buffer);
		return (NULL);
	}
	return (read_line_chars(current_node));
}

/**
 * free_lines - Frees the memory allocated for the linked list of line nodes.
 * @lines: The head of the linked list of line nodes.
 */
void free_lines(line_head *lines)
{
	/* int i = 0;*/
	line_head *current_node = lines;
	line_head *next_node;

	while (current_node != NULL)
	{
		next_node = current_node->next;
		free(current_node->buffer);
		free(current_node);
		current_node = next_node;
	}
}

/**
 * add_line_node - Adds new node to the linked list for a new file descriptor.
 * @lines: A pointer to the head pointer of the linked list.
 * @fd: The file descriptor associated with the new node.
 * @buffer: The buffer containing the read data for the file descriptor.
 * @bytes: The number of bytes read into the buffer.
 * Return: On success, it returns a pointer to the newly added node.
 * If memory allocation fails, it returns NULL.
 */
line_head *add_line_node(line_head **lines, const int fd, char *buffer,
						int bytes)
{
	line_head *new_node = malloc(sizeof(line_head));
		if (!new_node)
			return (NULL);

	new_node->fd = fd;
	new_node->bytes = bytes;
	new_node->buffer = buffer;
	new_node->next = *lines;
	*lines = new_node;

	return (new_node);
}

/**
 * read_line_chars - Extracts a complete line from the current node's buffer.
 * @current_node: The node containing the buffer to read from.
 * Return: On success, it returns a pointer to the line as a string.
 * If memory allocation fails, it returns NULL.
 */
char *read_line_chars(line_head *current_node)
{
char *line = NULL, *tmp = NULL;
int size = 0, bytes_c = 0, i, j;

while (current_node->bytes > 0)
{
	if (size < bytes_c + current_node->bytes + 1)
	{
		size += current_node->bytes + 1;
		tmp = malloc(sizeof(char) * size);

		if (!tmp)
			{
			free(line);
			return (NULL);
			}
		memcpy(tmp, line, bytes_c);
		memset(tmp + bytes_c, '\0', size - bytes_c);
		free(line);
		line = tmp;
	}
	for (i = 0; i < current_node->bytes; i++)
	{
		if (current_node->buffer[i] == '\n')
		{
			current_node->buffer[i++] = '\0';
			current_node->bytes -= i;
			memcpy(line + bytes_c, current_node->buffer, i);
			for (j = 0; i + j < READ_SIZE; j++, i++)
				current_node->buffer[j] = current_node->buffer[i];
			for (; j < READ_SIZE; j++)
				current_node->buffer[j] = '\0';

			return (line);
		}
	}
	memcpy(line + bytes_c, current_node->buffer, current_node->bytes);
	bytes_c += current_node->bytes;
	current_node->bytes = read(current_node->fd, current_node->buffer, READ_SIZE);
}
return (line);
}
