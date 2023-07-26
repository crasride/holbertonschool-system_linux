#include <stdio.h>
#include "hls.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/**
 * main - Entry point of the program.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line argument strings.
 * Return: 0 on success, non-zero on error.
 */


int main(int argc, char *argv[])
{
    int one_option = 0;
    int i, j;

    /* Buscamos la opción -1 en los argumentos */
    for (i = 1; i < argc; i++)
    {
        for (j = 0; argv[i][j] != '\0'; j++)
        {
            if (argv[i][j] != '-' || argv[i][j + 1] != '1' || argv[i][j + 2] != '\0')
            {
                break; // No es la opción -1
            }
            one_option = 1;
            break;
        }
    }

    if (argc == 1 || (argc == 2 && one_option))
    {
        list_files(".", argv[0], argc, one_option);
    }
    else
    {
        for (i = 1; i < argc; i++)
        {
            /* Saltamos la opción -1 */
            for (j = 0; argv[i][j] != '\0'; j++)
            {
                if (argv[i][j] != '-' || argv[i][j + 1] != '1' || argv[i][j + 2] != '\0')
                {
                    break;
                }
                continue;
            }

            list_files(argv[i], argv[0], argc, one_option);

            if (i < argc - 1)
            {
                printf("\n");
            }
        }
    }
    return 0;
}

