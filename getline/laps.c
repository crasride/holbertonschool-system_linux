#include <stdio.h>
#include <stdlib.h>
#include "laps.h"

static Car *cars;

/**
 * race_state - computes and displays the state of the race
 * @id: array of int for `identifiers` of each cars
 * @size: size of id's array
 */
void race_state(int *id, size_t size)
{
	if (size == 0)
	{
		free_allocated_memory();
		return;
	}

	for (size_t i = 0; i < size; i++)
	{
		update_laps(id[i]);
	}

	/* Print state of race */
	printf("Race state:\n");
	Car *current = cars;
	while (current != NULL)
	{
		printf("Car %d [%d laps]\n", current->id, current->laps);
		current = current->next;
	}
}

/**
 * update_laps - updates the laps of a car or creates a new car
 * @car_id: ID of the car to update the laps
 */
void update_laps(int car_id)
{
	Car *current = cars;
	int found = 0;

	while (current != NULL)
	{
		if (current->id == car_id)
		{
			current->laps++;
			found = 1;
			break;
		}
		current = current->next;
	}

	if (!found)
	{
		create_new_car(car_id);
	}
}

/**
 * create_new_car - creates a new car and adds it to the race
 * @car_id: ID of the new car
 */
void create_new_car(int car_id)
{
	/* Create a new car if the given id does not exist */
	Car *new_car = malloc(sizeof(Car));
	if (new_car == NULL)
	{
		fprintf(stderr, "Memory allocation error\n");
		exit(EXIT_FAILURE);
	}

	new_car->id = car_id;
	new_car->laps = 0;
	new_car->next = cars;
	cars = new_car;

	/* Print new car joins the race */
	printf("Car %d joined the race\n", car_id);
}

/**
 * free_allocated_memory - frees all allocated memory for cars
 */
void free_allocated_memory(void)
{
	/* Free all allocated memory */
	while (cars != NULL)
	{
		Car *temp = cars;
		cars = cars->next;
		free(temp);
	}
}
