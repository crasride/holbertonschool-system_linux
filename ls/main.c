	#include <stdio.h>
	#include <stdlib.h>
	#include "hls.h"
	#include <string.h>

	/**
	 * main - Entry point of the program.
	 * @argc: Number of command-line arguments.
	 * @argv: Array of command-line argument strings.
	 * Return: 0 on success, non-zero on error.
	 */

	int my_strcmp(const char *str1, const char *str2) {
	while (*str1 != '\0' && *str1 == *str2) {
		str1++;
		str2++;
	}
	return *str1 - *str2;
	}

	int main(int argc, char *argv[])
	{
	int i, k;
	int display_one_per_line = 0;
	int show_hidden = 0;
	int show_almost_all = 0;

	for (i = 1; i < argc; i++)
	{
		if (my_strcmp(argv[i], "-1") == 0)
		{
			display_one_per_line = 1;
			for (k = i; k < argc - 1; k++)
			{
				argv[k] = argv[k + 1];
			}
			argc--;
			i--;
		}
		else if (my_strcmp(argv[i], "-a") == 0)
		{
			show_hidden = 1;
			for (k = i; k < argc - 1; k++)
			{
				argv[k] = argv[k + 1];
			}
			argc--;
			i--;
		}
		else if (my_strcmp(argv[i], "-A") == 0)
		{
			show_almost_all = 1;
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
		list_files(".", argv[0], argc, display_one_per_line, show_hidden, show_almost_all, &list);
		free_entry_list(&list);
	}
	else
	{
		int files_listed = 0; /* Flag to indicate if at least one file has been listed in the current directory */

		for (i = 1; i < argc; i++)
		{
			struct EntryList list;
			list.head = NULL;
			list.count = 0;
			list_files(argv[i], argv[0], argc, display_one_per_line, show_hidden, show_almost_all, &list);
			free_entry_list(&list);

			if (i < argc - 1)
			{
				if (files_listed)
				{
					printf("\n"); /* Print line break only when files have been listed */
				}
				files_listed = 1; /* Set the flag to 1 to indicate that files in this directory have been listed */
			}
		}
	}
	return (0);
	}

