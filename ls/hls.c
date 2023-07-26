#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "hls.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * list_files - Function that lists the files in a directory excluding hidden
 * ones (those starting with '.').
 * @path: The path of the directory to list.
 * @program_name: The name of the program (argv[0]).
 */

void list_files(const char *path, const char *program_name, int num_args)
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
			fprintf(stderr, "%s: cannot open directory %s: Permission denied", program_name, path);
			perror("");
			exit(EXIT_FAILURE);
		}
		else
		{
			fprintf(stderr, "%s: cannot access %s: ", program_name, path);
			perror("");
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
			strcpy(files[num_files], ent->d_name);
			num_files++;
		}
	}
	closedir(dir);

	if (num_files == 0)
	{
		fprintf(stderr, "%s: %s: No such file or directory\n", program_name, path);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < num_files; i++)
	{
		printf("%s  ", files[i]);
	}
	printf("\n");
	}
