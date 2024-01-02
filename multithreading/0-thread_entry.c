
#include "multithreading.h"

/**
* thread_entry - Entry point for a new thread
*
* @arg: Argument passed to the thread
*
* Return: NULL
*/
void *thread_entry(void *arg)
{
	char *message = (char *)arg;

	if (message != NULL)
	{
		printf("%s\n", message);
	}
	pthread_exit(NULL);
}
