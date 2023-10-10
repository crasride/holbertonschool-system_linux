#include "signals.h"


/**
* main - Sends a SIGINT signal to a process with the specified PID.
*
* @argc: Number of command line arguments.
* @argv: Array of character strings containing command line arguments.
* Return: EXIT_SUCCESS if the operation is successful, EXIT_FAILURE in case of
* error or incorrect arguments.
*/
int main(int argc, char *argv[])
{

	pid_t target_pid;

	if (argc != 2)
	{
		printf("Usage: %s <pid>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	target_pid = atoi(argv[1]);

	if (target_pid == 0)
	{
		printf("Invalid PID: 0\n");
		return (EXIT_FAILURE);
	}

	if (kill(target_pid, SIGINT) == -1)
	{
		perror("kill");
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);
}
