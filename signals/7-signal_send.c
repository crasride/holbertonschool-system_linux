#include "signals.h"

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s <pid>\n", argv[0]);
		return (EXIT_FAILURE);
	}

	pid_t target_pid = atoi(argv[1]);

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
