#ifndef RACING_CARS
#define RACING_CARS

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * struct Car - struct to record laps by cars id
 * @id: car identifier
 * @laps: number of laps completed
 * @next: pointer to next car struct
*/

typedef struct Car
{
	int id;
	int laps;
	struct Car *next;
} Car;

void race_state(int *id, size_t size);
void update_laps(int car_id);
void create_new_car(int car_id);
void free_allocated_memory(void);

#endif /* RACING_CARS */
