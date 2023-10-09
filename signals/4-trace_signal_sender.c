#include "signals.h"

static pid_t sender_pid;

/**
* sigquit_handler - Signal handler for SIGQUIT.
*
* @signum: The received signal number (must be SIGQUIT).
*/
void sigquit_handler(int signum)
{
	if (signum == SIGQUIT)
	{
		printf("SIGQUIT sent by %i\n", sender_pid);
	}
}

/**
* trace_signal_sender - Configures a signal handler for SIGQUIT that prints
* the sender's PID when SIGQUIT is received.
*
* Return: 0 if the signal handler configuration was successful, -1 on failure.
*/
int trace_signal_sender(void)
{
	struct sigaction sa;

	sa.sa_handler = sigquit_handler;
	sa.sa_flags = SA_SIGINFO;

	if (sigaction(SIGQUIT, &sa, NULL) == -1)
	{
		return (-1);
	}

	return (0);
}
