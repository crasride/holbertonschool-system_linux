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
	if (argc == 1)
	{
		list_files(".");
	}
	else
	{
		for (int i = 1; i < argc; i++)
			list_files(argv[i]);
	}
	return (0);
}
