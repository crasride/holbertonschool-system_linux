#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include "signals.h"

static volatile pid_t sender_pid;

void sigquit_handler(int signum)
{
	if (signum == SIGQUIT)
	{
		printf("SIGQUIT sent by %i\n", sender_pid);
	}
}

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
