#include "signals.h"

/**
* sigint_handler - Handler function for SIGINT
* @signum: The signal number received (should be SIGINT)
*/
void sigint_handler(int signum)
{
	printf("Gotcha! [%d]\n", signum);
	fflush(stdout);
}

/**
* handle_signal - Sets a handler for the SIGINT signal
* Return: 0 on success, -1 on error
*/
int handle_signal(void)
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		return (-1);
	return (0);
}
