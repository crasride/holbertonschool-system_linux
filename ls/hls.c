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

void list_files(const char *path, const char *program_name)
{
    DIR *dir;
    struct dirent *ent;
    char files[1000][256];
    int num_files = 0;

    dir = opendir(path);
    if (dir == NULL)
    {
        fprintf(stderr, "%s: cannot access %s: %s\n", program_name, path, strerror(errno));
        exit(EXIT_FAILURE);
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

    for (int i = 0; i < num_files; i++)
    {
        printf("%s  ", files[i]);
    }
    printf("\n");
}
