#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

/**
 * main - Entry point
 *
 * Return: Always 0 on success.
 */
int main(void)
{
DIR *dir;
struct dirent *ent;

dir = opendir(".");
if (dir == NULL)
{
perror("opendir");
exit(EXIT_FAILURE);
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

return (0);
}
