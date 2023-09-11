#include <elf.h>
#include "hreadelf.h"
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	FILE *file = NULL;
	Elf32_Header elf32;
	Elf64_Header elf64;

	if (argc != 2)
	{
		fprintf(stderr, "Uso: %s elf_filename\n", argv[0]);
		return (1);
	}

	file = fopen(argv[1], "rb");

	if (!file)
	{
		perror("Error al abrir el archivo");
		return (1);
	}

	fread(&elf32, sizeof(Elf32_Header), 1, file);

	fread(&elf64, sizeof(Elf64_Header), 1, file);

	fclose(file);

	printf("Encabezado ELF de 32 bits:\n");

	printf("Encabezado ELF de 64 bits:\n");

	return (0);
}
