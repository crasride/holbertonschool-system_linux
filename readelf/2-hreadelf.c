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
	ElfHeader elf_header;
	int is_32bit = 0, value = 0;

	check_command(argc, argv);
	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		perror("No se puede abrir el archivo");
		return (1);
	}
	fread(&elf_header, sizeof(ElfHeader), 1, file);
	if (elf_header.ehdr.ehdr32.e_ident[EI_CLASS] == ELFCLASS32)
	{
		is_32bit = 1;
		if (elf_header.ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
			read_elf32_be_header(&elf_header.ehdr.ehdr32);
	}
	else if (elf_header.ehdr.ehdr64.e_ident[EI_CLASS] == ELFCLASS64)
		is_32bit = 0;
	else
	{
		fclose(file);
		return (1);
	}
	if (is_32bit ? elf_header.ehdr.ehdr32.e_phoff == 0
		: elf_header.ehdr.ehdr64.e_phoff == 0)
	{
		printf("\nThere are no program headers in this file.\n");
		return (0);
	}
	print_elf_info(&elf_header, is_32bit);
	fseek(file, (is_32bit ? elf_header.ehdr.ehdr32.e_phoff
			: elf_header.ehdr.ehdr64.e_phoff), SEEK_SET);
	value = print_program_info(file, elf_header, is_32bit);
	if (value != 0)
		return (value);
	select_type_elf_file(file, &elf_header, is_32bit);
	fclose(file);
	return (0);
}

/**
* select_type_elf_file - Select the appropriate processing method based
* on ELF file type.
* This function determines the ELF file type (32-bit or 64-bit) and endianness
* (little-endian or big-endian)
* and selects the appropriate processing method for generating the
* section-to-segment mapping.
* @file:       A pointer to the open ELF file.
* @elf_header: pointer ElfHeader structure containing ELF header information.
* @is_32bit: flag r the ELF file is 32-bit (if true) or 64-bit (if false).
*/
void select_type_elf_file(FILE *file, ElfHeader *elf_header, int is_32bit)
{
	/* Section to Segment mapping */
	if (is_32bit)
	{
		if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2LSB)
		{
			createSectionToSegmentMapping32(file, elf_header, is_32bit);
		}
		else if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
		{
			createSectionToSegmentMapping32(file, elf_header, is_32bit);
		}
	}
	else if (elf_header->ehdr.ehdr64.e_ident[EI_CLASS] == ELFCLASS64)
	{
		is_32bit = 0;
		createSectionToSegmentMapping64(file, elf_header, is_32bit);
	}
}


int print_program_info(FILE *file, ElfHeader elf_header, int is_32bit)
{
	int i = 0;
	char interp[MAX_INTERP_SIZE];

	for (i = 0; i < (is_32bit ? elf_header.ehdr.ehdr32.e_phnum : elf_header.ehdr.ehdr64.e_phnum); i++)
	{
		if (is_32bit)
		{
			Elf32_Phdr program_header;
			fread(&program_header, sizeof(Elf32_Phdr), 1, file);
			if (elf_header.ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
				read_elf32_be_prog(&program_header);
			print_program_header_info_32(&program_header);

			if (program_header.p_type == PT_INTERP)
			{
				if (program_header.p_filesz <= MAX_INTERP_SIZE)
				{
					long current_pos = ftell(file);
					fseek(file, program_header.p_offset, SEEK_SET);
					fread(interp, program_header.p_filesz, 1, file);
					interp[program_header.p_filesz] = '\0';
					print_interpreter_info(interp);
					fseek(file, current_pos, SEEK_SET);
				}
				else
				{
					fprintf(stderr, "MAX_INTERP_SIZE.\n");
					fclose(file);
					return (1);
				}
			}
		}
		else
		{
			Elf64_Phdr program_header;
			fread(&program_header, sizeof(Elf64_Phdr), 1, file);
			print_program_header_info_64(&program_header);

			if (program_header.p_type == PT_INTERP)
			{
				if (program_header.p_filesz <= MAX_INTERP_SIZE)
				{
					long current_pos = ftell(file);
					fseek(file, program_header.p_offset, SEEK_SET);
					fread(interp, program_header.p_filesz, 1, file);
					interp[program_header.p_filesz] = '\0';
					print_interpreter_info(interp);
					fseek(file, current_pos, SEEK_SET);
				}
				else
				{
					fprintf(stderr, "MAX_INTERP_SIZE.\n");
					fclose(file);
					return (1);
				}
			}
		}
	}
	return (0);
}

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
		exit (1);
	}
}


