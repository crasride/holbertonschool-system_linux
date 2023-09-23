#include "hreadelf.h"


/**
* check_command - Check the validity of command-line arguments.
* This function checks if the number of command-line arguments is correct
* (exactly 2 arguments expected).
* If the number of arguments is not 2, it prints an error message to standard
* error and exits the program with an error code.
*
* @argc: The number of command-line arguments.
* @argv: An array of strings containing the command-line arguments.
*/
void check_command(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Uso: %s elf_filename\n", argv[0]);
		exit(1);
	}
}


/**
* check_open_file - Check if a file pointer is valid and exit if it's not.
*
* This function checks if a given file pointer is valid (i.e., not NULL).
* If the file
* pointer is NULL, it prints an error message indicating that the file cannot
* be opened and then exits the program with an error code.
*
* @file: A pointer to a FILE structure representing the open file.
*/
void check_open_file(FILE *file)
{
	if (file == NULL)
	{
		perror("No se puede abrir el archivo");
		exit(1);
	}
}
