#include <stdio.h>
#include <stdlib.h>
#include "laps.h"

static Car *cars;
void race_state(int *id, size_t size)
{
    Car *current = cars;
    size_t i; // Mover la declaración de i aquí

    if (size == 0)
    {
        /* Free all allocated memory size is 0 */
        while (cars != NULL)
        {
            Car *temp = cars;
            cars = cars->next;
            free(temp);
        }
        return;
    }

    for (i = 0; i < size; i++)
    {
        current = cars;
        Car *prev = NULL;
        int found = 0;

        /* Check car with the given id exists */
        while (current != NULL)
        {
            if (current->id == id[i])
            {
                current->laps++;
                found = 1;
                break;
            }
            prev = current;
            current = current->next;
        }

        if (!found)
        {
            /* Create a new car if the given id not exist */
            Car *new_car = malloc(sizeof(Car));
            if (new_car == NULL)
            {
                fprintf(stderr, "Memory allocation error\n");
                exit(EXIT_FAILURE);
            }
            new_car->id = id[i];
            new_car->laps = 0;
            new_car->next = NULL;

            if (prev == NULL)
            {
                cars = new_car;
            }
            else
            {
                prev->next = new_car;
            }

            /* Print new car joins the race*/
            printf("Car %d joined the race\n", id[i]);
        }
    }

    /* Print state of race */
    printf("Race state:\n");
    current = cars;
    while (current != NULL)
    {
        printf("Car %d [%d laps]\n", current->id, current->laps);
        current = current->next;
    }
}
