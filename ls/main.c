#include <stdio.h>
#include "hls.h"
#include <string.h>

/**
 * main - Entry point of the program.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line argument strings.
 * Return: 0 on success, non-zero on error.
 */

int main(int argc, char *argv[])
{
	int i;
	int display_one_per_line = 0;

	

	/* Process the files and directories passed as arguments */
	if (argc == 1)
	{
		list_files(".", argv[0], argc, display_one_per_line);
	}
	else
	{
		for (i = 1; i < argc; i++)
		{
			list_files(argv[i], argv[0], argc, display_one_per_line);
			if (i < argc - 1 && display_one_per_line)
			{
				printf("\n");
			}
		}
	}
	return (0);
}
