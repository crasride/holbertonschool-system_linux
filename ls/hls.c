#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "hls.h"
#include <string.h>

/**
 * list_files - Function that lists the files in a directory excluding hidden
 * ones (those starting with '.').
 * @path: The path of the directory to list.
 */

#include <sys/stat.h>

void list_files(const char *path)
{
	struct stat path_stat;
	DIR *dir;
	struct dirent *ent;

	if (stat(path, &path_stat) != 0)
	{
		/* File/directory doesn't exist or inaccessible */
		fprintf(stderr, "./hls_01: cannot access %s: No such file or directory\n", path);
		return;
	}

	if (S_ISDIR(path_stat.st_mode))
	{
		/* It's a directory, proceed with listing files */
		dir = opendir(path);
		if (dir == NULL)
		{
			fprintf(stderr, "./hls_01: cannot access %s: No such file or directory\n", path);
			return;
		}

		while ((ent = readdir(dir)) != NULL)
		{
			if (ent->d_name[0] != '.')
			{
				printf("%s  ", ent->d_name);
			}
		}
		printf("\n");
		closedir(dir);
	}
	else
	{
		/* It's a file, just print the file name */
		printf("%s\n", path);
	}
}
