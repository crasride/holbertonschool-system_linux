#include <stdio.h>
#include "hls.h"

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
	int j;


	/* Check for the "-1" option in the command-line arguments */
	for (i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == '1' && argv[i][2] == '\0')
		{
			display_one_per_line = 1;
			/* Remove the "-1" option from the arguments */
			for (j = i; j < argc - 1; j++)
			{
				argv[j] = argv[j + 1];
			}
			argc--; /* Decrement the argument count to exclude the "-1" option*/
			i--; /* Adjust the index to review the current position again */
		}
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
