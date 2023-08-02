#ifndef _GETLINE_H
#define _GETLINE_H

#ifndef READ_SIZE
#define READ_SIZE 1024
#endif

/**
 * struct line_head - struct of linked list to store our chunks
 * of lines being read
 * @fd: file descriptor read
 * @bytes: bytes read
 * @buffer: pointer to buffer characters
 * @next: pointer to next struct
 */

typedef struct line_node {
	int fd;
	int bytes;
	char *buffer;
	struct line_node *next;
} line_head;


/* Function for our homemade getline */
char *_getline(const int fd);
void free_lines(line_head *lines);
line_head *add_line_node(line_head **lines, const int fd, char *buffer,
						int bytes);
char *read_line_chars(line_head *current_node);

#endif /* _GETLINE_H */
