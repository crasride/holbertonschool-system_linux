#include "signals.h"

/**
* print_hello - Nuestro manejador de señales personalizado.
*
* @signum: El número de señal recibida.
* Return: void
*/
void print_hello(int signum)
{
	(void)signum;
	printf("Hello :)\n");
	fflush(stdout);
}

/**
* set_print_hello - Establece el manejador de señal para SIGINT (Ctrl+C) para
* imprimir Hello :)
* en la consola.
* Return: void
*/
void set_print_hello(void)
{
	signal(SIGINT, print_hello);
}
