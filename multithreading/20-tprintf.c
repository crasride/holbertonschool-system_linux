#include "multithreading.h"

static pthread_mutex_t mutex;

/**
* mutex_destroy - Destructor function to destroy the mutex.
* It is registered as a destructor to ensure cleanup at program exit
*/
__attribute__((destructor))void mutex_destroy(void)
{
	if (pthread_mutex_destroy(&mutex))
		perror(NULL);
}

/**
* mutex_init - Constructor function to initialize the mutex.
* It is registered as a constructor to ensure proper initialization at program
* start
*/
__attribute__((constructor))void mutex_init(void)
{
	if (pthread_mutex_init(&mutex, NULL) != 0)
		perror(NULL);
}

/**
* tprintf - Thread-safe printf that prepends the thread ID to the output
* @format: String to print with printf format specifiers
*
* Return: Number of characters printed
*/
int tprintf(char const *format, ...)
{
	/* Initialize the arguments */
	va_list args;
	/* Store the number of printed characters */
	int result;

	/* Lock the mutex before entering the critical section */
	if (pthread_mutex_lock(&mutex) != 0)
		perror(NULL);

	/**
	* code that needs exclusive access to shared data
	* is already blocked by another thread, the current
	* thread will wait until the mutex is available
	*/

	/* Get the current thread ID */
	va_start(args, format);

	/* Store the number of printed characters */
	result = printf("[%lu] ", pthread_self());

	/* Print the thread ID and formatted string */
	result += vprintf(format, args);

	/* Get the current thread ID */
	va_end(args);

	/* Unlock mutex after exiting critical section */
	if (pthread_mutex_unlock(&mutex) != 0)
		perror(NULL);

	return (result);
}


