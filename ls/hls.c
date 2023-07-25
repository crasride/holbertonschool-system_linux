#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "hls.h"

/**
 * list_files_recursive - Helper function to list files in a directory recursively.
 * @path: The path of the directory to list.
 */

static void list_files_recursive(const char *path)
{
	DIR *dir;
	struct dirent *ent;

	dir = opendir(path);
	if (dir == NULL)
	{
		perror(path);
		return;
	}

	while ((ent = readdir(dir)) != NULL)
	{
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			continue;

		char entry_path[1024];
		snprintf(entry_path, sizeof(entry_path), "%s/%s", path, ent->d_name);

		struct stat st;
		if (lstat(entry_path, &st) == -1)
		{
			perror(entry_path);
			continue;
		}

		if (S_ISDIR(st.st_mode))
		{
			list_files_recursive(entry_path);
		}
		else
		{
			if (ent->d_name[0] != '.')
			{
				printf("%s  ", ent->d_name);
			}
		}
	}
	closedir(dir);
}

/**
 * list_files - Function that lists the files in a directory excluding hidden
 * ones (those starting with '.').
 * @path: The path of the directory to list.
 */

void list_files(const char *path)
{
	struct stat st;
	if (lstat(path, &st) == -1)
	{
		perror(path);
		return;
	}

	if (S_ISDIR(st.st_mode))
	{
		printf("%s:\n", path);
		list_files_recursive(path);
		printf("\n");
	}
	else
	{
		if (path[0] != '.')
		{
			printf("%s  ", path);
		}
	}
}
