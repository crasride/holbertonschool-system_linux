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
* wait_for_sigint - Waits until SIGINT (Interrupt) signal is received
*
* This function is responsible for suspending program execution
* until the SIGINT signal is received. It works recursively,
* checking if the SIGINT signal has been received. If the signal has not yet
* been received, the function uses the 'pause' function to suspend
* the current process and wait for the signal. When you finally receive
* the SIGINT signal, the function exits the recursion and allows the
* program continues its execution.
*/
void wait_for_sigint(void)
{
	if (!sigint_received)
	{
		pause();
		wait_for_sigint();
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

	wait_for_sigint();

	printf("Signal received\n");

	return (EXIT_SUCCESS);
}
