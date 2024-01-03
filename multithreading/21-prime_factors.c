#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/**
* is_prime - Checks if a number is prime
* @n: number to check
* Return: 1 if the number is prime, 0 otherwise
*/
int is_prime(unsigned long n)
{
	unsigned long i;

	if (n < 2)
		return (0);

	for (i = 2; i * i <= n; i++)
	{
		if (n % i == 0)
			return (0);
	}

	return (1);
}

/**
* add_factor - Add a factor to the list
*
* @factors: List of prime factors
* @num: Number to prime factor
*/
static void add_factor(list_t *factors, unsigned long num)
{
	unsigned long *factor = malloc(sizeof(unsigned long));

	if (factor)
	{
		*factor = num;
		list_add(factors, factor);
	}
}

/**
* handle_repeated_factors -Handles repeated prime factors (e.g. 2)
*
* @factors: List of prime factors
* @num: Number to factor
*/
static void handle_repeated_factors(list_t *factors, unsigned long *num)
{
	while (*num % 2 == 0)
	{
		add_factor(factors, 2);
		*num /= 2;
	}
}

/**
* prime_factors - Factoriza un número en una lista de factores primos
*
* @s: String representation of number
*
* Return: List to prime factor
*/
list_t *prime_factors(char const *s)
{
	unsigned long num = strtoul(s, NULL, 10);
	unsigned long i;
	list_t *factors = list_init(malloc(sizeof(list_t)));

	if (!factors)
		return (NULL);

	handle_repeated_factors(factors, &num);

	for (i = 3; i * i <= num; i += 2)
	{
		while (num % i == 0 && is_prime(i))
		{
			add_factor(factors, i);
			num /= i;
		}
	}

	if (num > 2)
		add_factor(factors, num);

	return (factors);
}
