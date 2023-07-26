#include <stdio.h>
#include "hls.h"
#include <unistd.h>
#include <stdlib.h>

/**
 * main - Entry point of the program.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line argument strings.
 * Return: 0 on success, non-zero on error.
 */

int main(int argc, char *argv[])
{
	int one_option = 0;
	int opt;
	int i;

	while ((opt = getopt(argc, argv, "1")) != -1)
	{
		switch (opt)
		{
		case '1':
			one_option = 1;
			break;
		default:
			fprintf(stderr, "Uso: %s [-1] [ARCHIVO]...\n", argv[0]);
			exit(EXIT_FAILURE);
		}
	}



	if (argc == 1)
	{
		list_files(".", argv[0], argc, one_option);
	}
	else
	{
		for (i = 1; i < argc; i++)
		{
			list_files(argv[i], argv[0], argc, one_option);
			if (i < argc - 1)
			{
				printf("\n");
			}

		}
	}
	return (0);
}
