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
* prime_factors - Factorize a number into a list of prime factors
*
* @s: String representation of the number
*
* Return: List of prime factors
*/
list_t *prime_factors(char const *s)
{

	unsigned long num = strtoul(s, NULL, 10);
	unsigned long i;
	list_t *factors = list_init(malloc(sizeof(list_t)));

	if (!factors)
		return (NULL);

	while (num % 2 == 0)
	{
		unsigned long *factor = malloc(sizeof(unsigned long));

		if (!factor)
		{
			list_destroy(factors, free);
			free(factors);
			return (NULL);
		}

		*factor = 2;
		list_add(factors, factor);
		num /= 2;
	}

	for (i = 3; i * i <= num; i += 2)
	{
		while (num % i == 0 && is_prime(i))
		{
			unsigned long *factor = malloc(sizeof(unsigned long));

			if (!factor)
			{
				list_destroy(factors, free);
				free(factors);
				return (NULL);
			}

			*factor = i;
			list_add(factors, factor);
			num /= i;
		}
	}

	if (num > 2)
	{
		unsigned long *factor = malloc(sizeof(unsigned long));

		if (!factor)
		{
			list_destroy(factors, free);
			free(factors);
			return (NULL);
		}

		*factor = num;
		list_add(factors, factor);
	}

	return (factors);
}
