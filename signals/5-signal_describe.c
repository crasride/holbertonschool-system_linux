#include "signals.h"

/**
* main - Displays the name of a signal based on its signal number.
*
* @argc: Number of command line arguments.
* @argv: Array of character strings containing command line arguments.
* Return: EXIT_SUCCESS if the operation is successful, EXIT_FAILURE in case of
* error or incorrect arguments.
*/
int main(int argc, char *argv[])
{
	int signum;

	if (argc != 2)
	{
		printf("Usage: %s <signum>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	signum = atoi(argv[1]);

	if (signum >= 1 && signum <= 64)
	{
		const char *signal_name = sys_siglist[signum];

		if (signal_name != NULL)
		{
			printf("%d: %s\n", signum, signal_name);
		}
		else
		{
			printf("%d: Unknown signal %d\n", signum, signum);
		}
	}
	else
	{
		printf("%d: Unknown signal %d\n", signum, signum);
	}

	return (EXIT_SUCCESS);
}
