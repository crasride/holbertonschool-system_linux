#include "signals.h"

/**
* sigint_handler - Handler function for SIGINT
* @signum: The signal number received (should be SIGINT)
*/
void sigint_handler(int signum)
{
	printf("Gotcha! [%d]\n", signum);
	/* is used to explicitly flush the buffer of n standard output (stdout)*/
	fflush(stdout);
}

/**
* handle_sigaction - Configure a custom signal handler for SIGINT using
* seguiraction.
*
* Return: 0 if the signal handler configuration was successful, -1 on failure.
*/
int handle_sigaction(void)
{
	struct sigaction sa;

	/* Configures the sa_handler signal handler for SIGINT. */
	sa.sa_handler = sigint_handler;
	/* Set sa flags to 0 (no special flags).*/
	sa.sa_flags = 0;

	/* Register the custom signal handler using seguiraction for SIGINT. */
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (-1);
	return (0);
}
