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
	int found_option_one = 0;

	/* Check for the "-1" option in the command-line arguments */
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-1") == 0)
		{
			display_one_per_line = 1;
			found_option_one = 1;

			argv[i] = NULL;
		}
	}

	if (found_option_one)
	{
		int new_argc = 0;
		for (i = 1; i < argc; i++)
		{
			if (argv[i] != NULL)
			{
				argv[new_argc++] = argv[i];
			}
		}
		argc = new_argc;
	}
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
			if (i < argc - 1)
			{
				printf("\n");
			}
		}
	}
	return (0);
}
