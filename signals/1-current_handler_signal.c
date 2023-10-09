#include <signal.h>
#include "signals.h"

void (*current_handler_signal(void))(int)
{
	void (*handler)(int) = signal(SIGINT, SIG_DFL);
	signal(SIGINT, handler);
	return (handler == SIG_ERR) ? NULL : handler;
}


