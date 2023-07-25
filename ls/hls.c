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
 */


void list_files(const char *path)
	{
	DIR *dir;
	struct dirent *ent;


	dir = opendir(path);
	if (dir != NULL)
	{

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

		fprintf(stderr, "./hls_01: cannot access %s: %s\n", path, strerror(errno));
	}
	}
