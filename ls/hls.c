#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "hls.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <pwd.h>
#include <time.h>


void print_formatted_time(time_t mod_time) {
    char *mod_time_str = ctime(&mod_time);
    char formatted_time[17];
    memcpy(formatted_time, mod_time_str + 4, 12);
    formatted_time[12] = '\0';
    printf("%s", formatted_time);
}

/**
 * my_strlen - Function to calculate the length of a string.
 * @str: The input string.
 * Return: The length of the string.
 */
size_t my_strlen(const char *str)
{
	size_t len = 0;
	while (str[len] != '\0')
	{
		len++;
	}
	return (len);
}

/**
 * my_strcpy - Function to copy a string.
 * @dest: Destination buffer where the string will be copied.
 * @src: Source string to be copied.
 * Return: Pointer to the destination buffer.
 */
char *my_strcpy(char *dest, const char *src)
{
	char *dest_start = dest;

	while ((*dest++ = *src++))
		;

	return (dest_start);
}

/**
 * list_files - Function that lists the files in a directory excluding hidden
 * ones (those starting with '.').
 * @path: The path of the directory to list.
 * @program_name: The name of the program (argv[0]).
 * @num_args: The number of arguments.
 * @display_one_per_line: Whether to display one entry per line.
 * @list: Pointer to the EntryList to store the entries.
 * @show_hidden: Whether to show hidden
 */
void list_files(const char *path, const char *program_name, int num_args, int display_one_per_line, int show_hidden, int show_almost_all, int detailed_listing, struct EntryList *list)
{
	DIR *dir;
	struct dirent *ent;
	struct Entry *current = NULL;

	/* Check if we are listing a regular file */
	struct stat file_stat;

	if (lstat(path, &file_stat) == 0 && S_ISREG(file_stat.st_mode))
	{
		printf("%s\n", path);
		return;
	}

	/* Print directory name if multiple directories are listed */
	if (num_args > 2)
	{
		printf("%s:\n", path);
	}

	dir = opendir(path);
	if (dir == NULL)
	{
		if (errno == EACCES)
		{
			fprintf(stderr, "%s: cannot open directory %s: ", program_name, path);
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

	while ((ent = readdir(dir)) != NULL)
	{
		if (!show_hidden && ent->d_name[0] == '.')
		{
			if (show_almost_all)
			{
				if (ent->d_name[1] == '\0' || (ent->d_name[1] == '.' && ent->d_name[2] == '\0'))
				{
					continue; /* Skip "." and ".." when using -A*/
				}
			}
			else
			{
				continue; /* Skip hidden files and directories when using -a*/
			}
		}

		if (show_hidden || show_almost_all || ent->d_name[0] != '.')
		{
			char full_path[1024];
			struct stat file_stat;

			my_strcpy(full_path, path);
			my_strcpy(full_path + my_strlen(full_path), "/");
			my_strcpy(full_path + my_strlen(full_path), ent->d_name);

			if (lstat(full_path, &file_stat) == 0)
			{
				/* Add the entry to the linked list */
				add_entry_to_list(list, ent->d_name, file_stat.st_mode, (ent->d_name[0] == '.'));
			}
			else
			{
				fprintf(stderr, "%s: cannot access %s/%s: ", program_name, path, ent->d_name);
				perror("");
				exit(EXIT_FAILURE);
			}
		}
	}

	closedir(dir);

	current = list->head;

	if (list->count == 0)
	{
		fprintf(stderr, "%s: %s: No such file or directory\n", program_name, path);
		exit(EXIT_FAILURE);
	}

	if (detailed_listing)
	{
		/* Print detailed information when using -l */
		while (current != NULL)
{
    char full_path[1024];
    struct stat file_stat;
    time_t mod_time;

    my_strcpy(full_path, path);
    my_strcpy(full_path + my_strlen(full_path), "/");
    my_strcpy(full_path + my_strlen(full_path), current->name);

    if (lstat(full_path, &file_stat) == 0)
    {
        mod_time = file_stat.st_mtime;

        printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
        printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
        printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
        printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
        printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
        printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
        printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
        printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
        printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
        printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");
        printf(" %lu", (unsigned long)file_stat.st_nlink);
        printf(" %u", file_stat.st_uid);
        printf(" %u", file_stat.st_gid);
        printf(" %ld", (long)file_stat.st_size);
        print_formatted_time(mod_time);
        printf(" %s\n", current->name);
    }
            else
			{
				fprintf(stderr, "%s: cannot access %s/%s: ", program_name, path, current->name);
				perror("");
				exit(EXIT_FAILURE);
			}

			current = current->next;
		}
	}
	else if (display_one_per_line)
	{
		/* Print filenames with one per line when using -1 */
		while (current != NULL)
		{
			printf("%s\n", current->name);
			current = current->next;
		}
	}
	else
	{
		/* Print filenames with spaces when not using -l or -1*/
		while (current != NULL)
		{
			printf("%s  ", current->name);
			current = current->next;
		}
		printf("\n");
	}
}

/**
 * add_entry_to_list - Function to add a new entry to the linked list.
 * @list: Pointer to the EntryList where the entry will be added.
 * @name: Name of the entry to be added.
 * @st_mode: Mode of the entry (permissions, type, etc.).
 * @is_hidden:
 * This function adds a new entry to the linked list, allocating memory
 * for the new entry and copying the provided name and st_mode.
 */
void add_entry_to_list(struct EntryList *list, const char *name, mode_t st_mode, int is_hidden)

{
	struct Entry *new_entry = (struct Entry *)malloc(sizeof(struct Entry));

	if (new_entry == NULL)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	my_strcpy(new_entry->name, name);
	new_entry->st_mode = st_mode;
	new_entry->is_hidden = is_hidden;
	new_entry->next = list->head;
	list->head = new_entry;
	list->count++;
}

/**
 * free_entry_list - Function to free the memory used by the linked list.
 * @list: Pointer to the EntryList to be freed.
 *
 * This function frees the memory used by the linked list of entries,
 * including the memory allocated for each individual entry.
 */

void free_entry_list(struct EntryList *list)

{
	struct Entry *current = list->head;
	while (current != NULL)
	{
		struct Entry *next = current->next;
		free(current);
		current = next;
	}
	list->head = NULL;
	list->count = 0;
}
