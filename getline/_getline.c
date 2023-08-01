#include <unistd.h>
#include <stdlib.h>
#include "_getline.h"

#ifndef READ_SIZE
#define READ_SIZE 1024
#endif


/**
 * read_data -rtrtr
 * @fd:trrtrtr
 * @buffer:trtr
 * @bytes_to_read:rtrtr
 * Return:rtrtr
*/
static ssize_t read_data(const int fd, char *buffer, ssize_t bytes_to_read)
{
	ssize_t bytes_read = read(fd, buffer, bytes_to_read);

	return (bytes_read);
}

/* find the end of the line in the buffer */
static ssize_t find_end_of_line(char *buffer, ssize_t start_position,
									ssize_t bytes_read)
{
	ssize_t i = start_position;

	while (i < bytes_read && buffer[i] != '\n')
		i++;
	return (i);
}

/* find the end of the line in the buffer*/
static char *allocate_and_copy_line(char *line, ssize_t line_length,
										char *buffer, ssize_t start_position,
											ssize_t end_position)
{
	char *line_realloc = realloc(line,
							line_length + end_position - start_position + 1);
	if (!line_realloc)
	{
		free(line); /* Cleanup in case of allocation failure*/
		return (NULL);
	}

	line = line_realloc;

	ssize_t k, j;
	for (k = start_position, j = 0; k < end_position; k++, j++)
	{
		line[j] = buffer[k];
	}
	line[end_position - start_position] = '\0'; /* Null-terminate the line */

	return (line);
}


/**
 * _getline -rter
 * @fd:rtrt
 * Return:rtert
*/
char *_getline(const int fd)
{
	static char buffer[READ_SIZE];
	static ssize_t bytes_read;
	static ssize_t current_position;
	char *line = NULL;
	ssize_t i = current_position;
	ssize_t j = 0;

	while (1)
	{
		ssize_t end_position;
		/* Check if no more bytes in buffer,or if it's the first time reading */
		if (current_position == bytes_read)
		{
			bytes_read = read_data(fd, buffer, READ_SIZE);
			if (bytes_read <= 0)
				return (NULL); /* End of file or error, return NULL*/

			current_position = 0; /* Reset the buffer position */
			i = 0; /* Reset the line parsing index */
		}

		/* Find the end of the line */
		end_position = find_end_of_line(buffer, i, bytes_read);

		/* Calculate the length of the line and allocate memory for it*/
		/* null terminator */
		ssize_t line_length = end_position - current_position + 1;

		line = allocate_and_copy_line(line, j, buffer, current_position,
										end_position);

		/* Move to the next character after the newline */
		current_position = end_position + 1;

		/* If we found a newline, return the line */
		if (end_position < bytes_read
							|| (end_position == bytes_read && line_length > 1))
			return (line);
	}

	return (NULL);
}
