#include <elf.h>
#include "hreadelf.h"


int main(int argc, char *argv[])
{
	FILE *file = NULL;
	
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
		return (1);
	}
	file = fopen(argv[1], "rb");
	if (!file)
	{
		perror("Error opening the file");
		return (1);
	}
	fclose(file);
	return (0);
}
