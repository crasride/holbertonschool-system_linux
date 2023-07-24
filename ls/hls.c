#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

/**
 * main - Entry point
 * @argc: Number of command-line arguments
 * @argv: Array of command-line arguments
 *
 * Return: 0 on success, 1 on error.
 */
int main(int argc, char *argv[])
{
DIR *dir;
struct dirent *ent;
int i;

if (argc == 1)
{
// No arguments supplied, list the contents of the current directory
dir = opendir(".");
if (dir == NULL)
{
    perror("opendir");
    return 1;
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
// List contents of directories or files given as arguments
for (i = 1; i < argc; i++)
{
    dir = opendir(argv[i]);
    if (dir != NULL)
    {
        printf("%s:\n", argv[i]);
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
        struct stat st;
        if (stat(argv[i], &st) == 0 && S_ISREG(st.st_mode))
        {
            printf("%s\n", argv[i]);
        }
        else
        {
            fprintf(stderr, "%s: cannot access %s: No such file or directory\n", argv[0], argv[i]);
            return 1;
        }
    }
}
}
return 0;
}
