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
