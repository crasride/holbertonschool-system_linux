#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "hls.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * my_strcpy - Function to copy a string from source to destination.
 * @dest: The destination buffer.
 * @src: The source string to copy.
 * Return: Pointer to the destination buffer.
 */
char *my_strcpy(char *dest, const char *src)
{
	char *dest_ptr = dest;

	while (*src)
	{
		*dest = *src;
		dest++;
		src++;
	}

	*dest = '\0';

	return (dest_ptr);
}

/**
 * list_files - Function that lists the files in a directory excluding hidden
 * ones (those starting with '.').
 * @path: The path of the directory to list.
 * @program_name: The name of the program (argv[0]).
 * @num_args: The number of arguments.
 * @display_one_per_line: Whether to display one file per line.
 */
void list_files(const char *path, const char *program_name, int num_args, int display_one_per_line)
{
	DIR *dir;
	struct dirent *ent;
	char files[1000][256];
	int num_files = 0;
	int i;

	struct stat file_stat;
	if (lstat(path, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
	{
		printf("%s\n", path);
		return;
	}

	dir = opendir(path);
	if (dir == NULL)
	{
		if (errno == EACCES)
		{
			perror(program_name);
			exit(EXIT_FAILURE);
		}
		else
		{
			perror(program_name);
			exit(EXIT_FAILURE);
		}
	}

	if (num_args > 2)
	{
		printf("%s:\n", path);
	}

	while ((ent = readdir(dir)) != NULL)
	{
		if (ent->d_name[0] != '.')
		{
			size_t name_length = strlen(ent->d_name);
			if (name_length < sizeof(files[num_files]) - 1)
			{
				my_strcpy(files[num_files], ent->d_name);
				num_files++;
			}
		}
	}
	closedir(dir);

	if (num_files == 0)
	{
		fprintf(stderr, "%s: %s: No such file or directory\n", program_name, path);
		exit(EXIT_FAILURE);
	}

	if (display_one_per_line)
	{
		for (i = 0; i < num_files; i++)
		{
			printf("%s\n", files[i]);
		}
	}
	else
	{
		for (i = 0; i < num_files; i++)
		{
			printf("%s  ", files[i]);
		}
		printf("\n");
	}
}
