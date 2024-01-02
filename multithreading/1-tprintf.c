#include "multithreading.h"

/*
* uses va_list, va_start, va_arg, and va_end to work with variable arguments
* safely, it is more flexible and allows handling arguments of different types.
* https://en.cppreference.com/w/c/variadic
*/

/**
* tprintf - print a formatted string
*
* @format: string to print
*
* Return: number of characters printed
*/
int tprintf(char const *format, ...)
{
	/* Get the current thread ID */
	pthread_t tid = pthread_self();
	/* Initialize the arguments */
	va_list args;
	/* Store the number of printed characters */
	int result;
	/* Print the thread ID and formatted string */
	printf("[%lu] ", tid);
	va_start(args, format);
	result = vprintf(format, args);
	va_end(args);

	return (result);
}


