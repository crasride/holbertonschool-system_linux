#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
/**
 * main - entry point for hacking task2
 * @argc: count of arguments
 * @argv: array of arguments
 * @env: array of strings env
 * Return: 0 success, 1 failure (expanded)
 */

int main(int argc, char **argv, char **env)
{
	char *args[] = {"/usr/bin/readelf", "-W", "-l", "", NULL};

	if (argc != 2)
	{
		fprintf(stderr, "Usage: you know what to do\n");
		return (1);
	}
	args[3] = argv[1];
	if (execve("/usr/bin/readelf", args, env) == -1)
	{
		perror("");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
