#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "hreadelf.h"

/**
 * main - entry point of make 0
 * @argc: count of arguments
 * @argv: array of arguments
 * Return: 0 success, 1 otherwise
 */
int main(int argc, char *argv[])
{
	FILE *file = NULL;

	Elf32_Ehdr elf_header32;
	Elf64_Ehdr elf_header64;
	off_t section_table_offset;
	char *section_names = NULL;
	int is_32bit = 0; /* Variable para detectar si es un archivo de 32 bits */

	if (argc != 2)
		return (EXIT_SUCCESS);
	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		perror("No se puede abrir el archivo");
		return (1);
	}
	/* Leer el encabezado ELF principal */
	fread(&elf_header32, sizeof(Elf32_Ehdr), 1, file);
	/* Verificar si es un archivo ELF de 32 bits */
	if (elf_header32.e_ident[EI_CLASS] == ELFCLASS32)
	{
		is_32bit = 1;
		/* Detectar el endianness del archivo ELF de 32 bits */
		if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
			read_elf32_be_header(&elf_header32);
	}
	else
	{
		/* Retroceder al principio del archivo si no es un archivo de 32 bits */
		fseek(file, 0, SEEK_SET);
		fread(&elf_header64, sizeof(Elf64_Ehdr), 1, file);
	}
	/* Leer la posición de la tabla de secciones */
	section_table_offset = is_32bit ? elf_header32.e_shoff : elf_header64.e_shoff;
	section_names = set_section_names(is_32bit, file, elf_header32, elf_header64);
	print_header(is_32bit, file, elf_header32, elf_header64,
					section_table_offset);
	loop_print(is_32bit, file, elf_header32, elf_header64, section_names);
	fclose(file);
	return (0);
}

/**
 * read_elf32_be_section - Convert a 32-bit ELF section header from big-endian
 * to host byte order.
 * This function takes a pointer to a 32-bit ELF section header in big-endian
 * byte order
 * and converts various section header fields to the host byte order.It is used
 * to ensure
 * correct interpretation of the section header on the host system.
 *
 * @section_header32: pointer to a 32-bit ELF section header in big-endian
 * byte order.
 */
void read_elf32_be_section(Elf32_Shdr *section_header32)
{
	section_header32->sh_name = my_be32toh(section_header32->sh_name);
	section_header32->sh_type = my_be32toh(section_header32->sh_type);
	section_header32->sh_addr = my_be32toh(section_header32->sh_addr);
	section_header32->sh_offset = my_be32toh(section_header32->sh_offset);
	section_header32->sh_size = my_be32toh(section_header32->sh_size);
	section_header32->sh_entsize = my_be32toh(section_header32->sh_entsize);
	section_header32->sh_flags = my_be32toh(section_header32->sh_flags);
	section_header32->sh_link = my_be32toh(section_header32->sh_link);
	section_header32->sh_info = my_be32toh(section_header32->sh_info);
	section_header32->sh_addralign = my_be32toh(section_header32->sh_addralign);
}

/**
 * read_elf32_be_header - Convert a 32-bit ELF header from big-endian to host
 * byte order.
 * This function takes a pointer to a 32-bit ELF header big-endian byte order
 * and converts various header fields to the host byte order. It is used to
 * ensure
 * correct interpretation of the header on the host system.
 *
 * @ehdr: A pointer to a 32-bit ELF header in big-endian byte order.
 */
void read_elf32_be_header(Elf32_Ehdr *ehdr)
{
	ehdr->e_type = my_be16toh(ehdr->e_type);
	ehdr->e_machine = my_be16toh(ehdr->e_machine);
	ehdr->e_version = my_be32toh(ehdr->e_version);
	ehdr->e_entry = my_be32toh(ehdr->e_entry);
	ehdr->e_phoff = my_be32toh(ehdr->e_phoff);
	ehdr->e_shoff = my_be32toh(ehdr->e_shoff);
	ehdr->e_flags = my_be32toh(ehdr->e_flags);
	ehdr->e_ehsize = my_be16toh(ehdr->e_ehsize);
	ehdr->e_phentsize = my_be16toh(ehdr->e_phentsize);
	ehdr->e_phnum = my_be16toh(ehdr->e_phnum);
	ehdr->e_shentsize = my_be16toh(ehdr->e_shentsize);
	ehdr->e_shnum = my_be16toh(ehdr->e_shnum);
	ehdr->e_shstrndx = my_be16toh(ehdr->e_shstrndx);
}

/**
* set_section_names - Retrieve the section names string table from an ELF file
*
* This function seeks the section names string table in ELF file and returns
* a pointer to it.It can handle both 32-bit and 64-bit ELF files, adjusting its
* behavior based on the provided parameters.
*
* @is_32bit: Indicates whether the ELF file is 32-bit (true) or 64-bit (false)
* @file: A pointer to the open ELF file.
* @elf_header32: The 32-bit ELF header (if applicable).
* @elf_header64: The 64-bit ELF header (if applicable).
*
* Return: A pointer to the section names string table.
*/
char *set_section_names(int is_32bit, FILE *file, Elf32_Ehdr elf_header32,
						Elf64_Ehdr elf_header64)
{
	Elf32_Shdr section_header32;
	Elf64_Shdr section_header64;
	char *section_names = NULL;

	if (is_32bit)
	{
		if (elf_header32.e_ident[EI_DATA] == ELFDATA2LSB)
		{
			fseek(file, elf_header32.e_shoff + elf_header32.e_shstrndx *
					elf_header32.e_shentsize, SEEK_SET);
			section_names = get_section_name32(section_header32, file);
		}
		if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
		{
			read_elf32_be_section(&section_header32);
			fseek(file, elf_header32.e_shoff + elf_header32.e_shstrndx *
					elf_header32.e_shentsize, SEEK_SET);
			section_names = get_section_name32_big(section_header32, file);
		}
	}
	else
	{
		fseek(file, elf_header64.e_shoff + elf_header64.e_shstrndx *
				elf_header64.e_shentsize, SEEK_SET);
		section_names = get_section_name64(section_header64, file);
	}
	return (section_names);
}

/**
 * loop_print - Iterate through and print information about ELF section headers
 *
 * This function iterates through the section headers of an ELF (Executable and
 * Linkable Format) file, reading and printing details such as section names,
 * types,
 * addresses, offsets, sizes, and other attributes. It can handle both 32-bit
 * and 64-bit ELF files.
 *
 * @is_32bit: Indicates whether the ELF file is 32-bit (true) or 64-bit (false)
 * @file: A pointer to the open ELF file.
 * @elf_header32: The 32-bit ELF header (if applicable).
 * @elf_header64: The 64-bit ELF header (if applicable).
 * @section_names: A pointer to the section names string table.
 */
void loop_print(int is_32bit, FILE *file, Elf32_Ehdr elf_header32,
				Elf64_Ehdr elf_header64, char *section_names)
{
	int index = 0;

	if (is_32bit)
	{
		Elf32_Shdr section_header32;

		for (index = 0; index < elf_header32.e_shnum; index++)
		{
			char *name = "";

			fread(&section_header32, sizeof(Elf32_Shdr), 1, file);

			if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
				read_elf32_be_section(&section_header32);

			if (section_header32.sh_name)
				name = section_names + section_header32.sh_name;
			print_Section_Info_32bits(index, section_header32, name);
		}
		printKeyToFlags_32bits();
	}
	else
	{
		Elf64_Shdr section_header64;

		for (index = 0; index < elf_header64.e_shnum; index++)
		{
			char *name = "";

			fread(&section_header64, sizeof(Elf64_Shdr), 1, file);

			if (section_header64.sh_name)
				name = section_names + section_header64.sh_name;
			print_Section_Info_64bits(index, section_header64, name);
		}
		printKeyToFlags_64bits();
	}
}
