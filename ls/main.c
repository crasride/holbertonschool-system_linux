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
	int multiple_folders = 0;

	if (argc == 1)
	{
		list_files(".", argv[0]);
	}
	else
	{
		for (i = 1; i < argc; i++)
		{
			if (multiple_folders)
			{
				printf("\n");
			}
			list_files(argv[i], argv[0]);
			multiple_folders = 1;
		}
	}
	return 0;
}

