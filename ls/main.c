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

	if (argc == 1)
	{
		list_files(".", argv[0], argc);
	}
	else
	{
		for (i = 1; i < argc; i++)
		{
			list_files(argv[i], argv[0], argc);
			if (i < argc - 1)
			{
				printf("\n");
			}

		}
	}
	return (0);
}
