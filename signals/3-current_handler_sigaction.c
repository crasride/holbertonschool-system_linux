#include "signals.h"

/**
* current_handler_sigaction - Gets and returns the current signal handler for
* SIGINT using seguiraction.
*
* Return: A pointer to the current signal handling function for SIGINT
* If the current handle cannot be obtained or if it is an error, NULL is
* returned.
*/
void (*current_handler_sigaction(void))(int)
{
	struct sigaction current_action;

	if (sigaction(SIGINT, NULL, &current_action) == -1)
	{
		return (NULL);
	}

	return (current_action.sa_handler);
}
