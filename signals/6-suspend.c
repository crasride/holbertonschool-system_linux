#include "signals.h"

static int sigint_received;

/**
* sigint_handler - Handler function for SIGINT
* @signum: The signal number received (should be SIGINT)
*/
void sigint_handler(int signum)
{
	if (signum == SIGINT)
	{
		printf("Caught %d\n", signum);
		sigint_received = 1;
	}
}


/**
* main - Displays the name of a signal based on its signal number.
*
* Return: EXIT_SUCCESS if the operation is successful, EXIT_FAILURE in case of
* error or incorrect arguments.
*/
int main(void)
{
	struct sigaction sa;

	sa.sa_handler = sigint_handler;
	sa.sa_flags = 0;

	if (sigaction(SIGINT, &sa, NULL) == -1)
	{
		perror("sigaction");
		return (EXIT_FAILURE);
	}

	while (!sigint_received)
	{
		/* Wait for SIGINT */
		pause();
	}

	printf("Signal received\n");

	return (EXIT_SUCCESS);
}
