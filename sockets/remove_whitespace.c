#include "socket.h"

/**
* remove_whitespace - Remove leading and trailing whitespace from a string
* @str: The string to trim
* Return: The trimmed strin
*/
char *remove_whitespace(char *str)
{
	size_t len = strlen(str);
	char *end;

	if (len == 0)
		return (str);

	end = str + len - 1;
	while (end > str && isspace((unsigned char)*end))
		end--;
	*(end + 1) = '\0';

	while (*str && isspace((unsigned char)*str))
		str++;

	return (str);
}
