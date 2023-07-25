#ifndef HLS_H
#define HLS_H


/**
 * list_files - Function that lists the files in a directory excluding hidden
 * ones (those starting with '.').
 * @path: The path of the directory to list.
 * @program_name: The name of the program (argv[0]).
 */

void list_files(const char *path, const char *program_name);

#endif /* HLS_H */
