#include "signals.h"

int main(int argc, char *argv[])
{
	int signum;
	if (argc != 2)
	{
		printf("Usage: %s <signum>\n", argv[0]);
		return EXIT_FAILURE;
	}

	signum = atoi(argv[1]);

	if (signum >= 1 && signum <= 64)
	{
		const char *signal_name = sys_siglist[signum];
		if (signal_name != NULL)
		{
			printf("%d: %s\n", signum, signal_name);
		}
		else
		{
			printf("%d: Unknown signal %d\n", signum, signum);
		}
	}
	else
	{
		printf("%d: Unknown signal %d\n", signum, signum);
	}

	return EXIT_SUCCESS;
}
