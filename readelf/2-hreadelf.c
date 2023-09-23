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
	check_open_file(file);
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
	value = is_32bit ? print_program_info32(file, elf_header)
					: print_program_info64(file, elf_header);
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


/**
* print_program_info32 - Print information about program headers in
* a 32-bit ELF file.
* This function reads and prints information about program headers
* in a 32-bit ELF file.
* It iterates through the program headers, reads their contents, and extracts
* relevant information such as the program type and interpreter details.
*
* @file: A pointer to the open ELF file.
* @elf_header: An ElfHeader structure containing ELF header information.
* Return:  Returns 0 on success, and 1 if there is an error
* (e.g., exceeding MAX_INTERP_SIZE).
*/
int print_program_info32(FILE *file, ElfHeader elf_header)
{
	int i = 0;
	char interp[MAX_INTERP_SIZE];

	for (i = 0; i < elf_header.ehdr.ehdr32.e_phnum; i++)
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
	return (0);
}

/**
* print_program_info64 - Print info about program headers in a 64-bit ELF file.
*
* This function reads and prints information about program headers in a
* 64-bit ELF file.
* It iterates through the program headers, reads their contents, and extracts
* relevant information such as the program type and interpreter details.
*
* @file:  A pointer to the open ELF file.
* @elf_header: An ElfHeader structure containing ELF header information.
*
* Return: Returns 0 on success, and 1 if there is an error
* (e.g., exceeding MAX_INTERP_SIZE).
*/
int print_program_info64(FILE *file, ElfHeader elf_header)
{
	int i = 0;
	char interp[MAX_INTERP_SIZE];

	for (i = 0; i < elf_header.ehdr.ehdr64.e_phnum; i++)
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
	return (0);
}
