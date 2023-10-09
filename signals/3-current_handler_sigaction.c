#include "signals.h"

void (*current_handler_sigaction(void))(int)
{
	struct sigaction current_action;

	if (sigaction(SIGINT, NULL, &current_action) == -1)
	{
		return (NULL);
	}

	return (current_action.sa_handler);
}
