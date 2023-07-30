#include <stdio.h>
#include <stdlib.h>
#include "hls.h"

/**
 * main - Entry point of the program.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line argument strings.
 * Return: 0 on success, non-zero on error.
 */

int main(int argc, char *argv[])
{
	int i;
	int display_one_per_line = 0;
	int k;

	/* Check for the "-1" option in the command-line arguments */
	for (i = 1; i < argc; i++)
	{
		int j;
		int is_option_1 = 1;

		for (j = 0; argv[i][j] != '\0'; j++)
		{
			if (argv[i][j] != '-' && argv[i][j] != '1')
			{
				is_option_1 = 0;
				break;
			}
		}

		if (is_option_1)
		{
			display_one_per_line = 1;

			for (k = i; k < argc - 1; k++)
			{
				argv[k] = argv[k + 1];
			}
			argc--;
			i--;
		}
	}

	/* Process the files and directories passed as arguments */
	if (argc == 1)
	{
		struct EntryList list;
		list.head = NULL;
		list.count = 0;
		list_files(".", argv[0], argc, display_one_per_line, &list);
		free_entry_list(&list);
	}
	else
	{
		int files_listed = 0; /*Flag to indicate if at least one file has been listed in the current directory*/

		for (i = 1; i < argc; i++)
		{
			struct EntryList list;
			list.head = NULL;
			list.count = 0;
			list_files(argv[i], argv[0], argc, display_one_per_line, &list);
			free_entry_list(&list);

			if (i < argc - 1 && list.count > 0)
			{
				if (files_listed)
				{
					printf("\n"); /*Print line break only when files have been listed*/
				}
				files_listed = 1; /*Set the flag to 1 to indicate that files in this directory have been listed*/
			}
		}
	}
	return (0);
}
