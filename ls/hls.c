#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "hls.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * list_files - Function that lists the files in a directory excluding hidden
 * ones (those starting with '.').
 * @path: The path of the directory to list.
 */


void list_files(const char *path)
{
	DIR *dir;
	struct dirent *ent;
	struct stat path_stat;

	if (stat(path, &path_stat) != 0)
	{
		fprintf(stderr, "./hls_01: cannot access %s: No such file or directory\n", path);
		return;
	}

	dir = opendir(path);
	if (dir == NULL)
	{

		printf("%s\n", path);
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
