#ifndef HLS_H
#define HLS_H


/**
* list_files - Function that lists the files in a directory excluding hidden
* ones (those starting with '.').
* @path: The path of the directory to list.
* @program_name: The name of the program (argv[0]).
* @num_args: The number argument to the program.
*/

void list_files(const char *path, const char *program_name, int num_args, int display_one_per_line);

#endif /* HLS_H */
