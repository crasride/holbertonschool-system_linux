#include "hreadelf.h"

/**
* createSectionToSegmentMapping32 - Create and print a mapping of sections
* to segments in an ELF file (32-bit).
* This function reads the ELF file generates a mapping of sections to segments.
* It then prints this mapping to the standard output.
*
* @file:A pointer to the ELF file.
* @elf_header: elf_header A pointer to the ELF header structure.
* @is_32bit: A flag indicating whether the ELF file is 32-bit (1) or 64-bit (0).
*/
void createSectionToSegmentMapping32(FILE *file, ElfHeader *elf_header,
									int is_32bit)
{
	int i, j;
	Elf32_Shdr shstrtab_header;
	Elf32_Shdr section_headers[MAX_INTERP_SIZE];
	char *shstrtab = NULL;
	char sections[MAX_INTERP_SIZE] = "";

	SectionToSegmentMapping *mapping = (SectionToSegmentMapping *)malloc(elf_header->ehdr.ehdr32.e_phnum * sizeof(SectionToSegmentMapping));

	for (i = 0; i < elf_header->ehdr.ehdr32.e_phnum; i++)
	{
		mapping[i].segment_number = i;
		strcpy(mapping[i].sections, "");
	}

	/* Obtener la tabla de cadenas de secciones */
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->ehdr.ehdr64.e_shoff) + elf_header->ehdr.ehdr32.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);

	fread(&shstrtab_header, sizeof(Elf32_Shdr), 1, file);
	if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
		read_elf32_be_section(&shstrtab_header);

	shstrtab = (char *)malloc(shstrtab_header.sh_size);
	fseek(file, shstrtab_header.sh_offset, SEEK_SET);
	fread(shstrtab, shstrtab_header.sh_size, 1, file);

	/* Leer section headers */
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->ehdr.ehdr64.e_shoff), SEEK_SET);
	fread(section_headers, sizeof(Elf32_Shdr), elf_header->ehdr.ehdr32.e_shnum, file);

	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_phoff : elf_header->ehdr.ehdr64.e_phoff), SEEK_SET);
	for (i = 0; i < (is_32bit ? elf_header->ehdr.ehdr32.e_phnum : elf_header->ehdr.ehdr64.e_phnum); i++)
	{
		Elf32_Phdr program_header;
		fread(&program_header, sizeof(Elf32_Phdr), 1, file);
		if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
			read_elf32_be_prog(&program_header);
		strcpy(sections, "");

		for (j = 0; j < elf_header->ehdr.ehdr32.e_shnum; j++)
		{
			Elf32_Shdr section_header;
			const char *section_name;

			section_header = section_headers[j];
			if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
				read_elf32_be_section(&section_header);
			/* Obtener el nombre de la sección utilizando la tabla de cadenas de secciones */
			section_name = shstrtab + section_header.sh_name;
			/* Omitir la asignación de estas secciones específicas */
			if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2LSB)
			{
				if (strcmp(section_name, ".gnu_debuglink") == 0 || strcmp(section_name, ".shstrtab") == 0 || strcmp(section_name, ".tm_clone_table") == 0)
				{
					continue;
				}
			}

			if (section_header.sh_addr >= program_header.p_vaddr && section_header.sh_addr + section_header.sh_size <= program_header.p_vaddr + program_header.p_memsz)
			{
				if (strlen(sections) > 0)
				{
					strcat(sections, " ");
				}
				strcat(sections, section_name);
			}

		}
		if (strlen(sections) > 0)
		{
			strcpy(mapping[i].sections, sections);
		}
	}
	free(shstrtab);
	/* Imprimir la tabla de mapeo de secciones a segmentos */
	print_mapping64(mapping, elf_header->ehdr.ehdr32.e_phnum);
	free(mapping);
}

/**
* print_mapping32 - Print the mapping of sections to segments in an ELF file
*
* This function takes an array of SectionToSegmentMapping structures and number
* of segments as input and prints the mapping in a human-readable format to
* the standard output.
* @mapping: An array of SectionToSegmentMapping structures.
* @num_segments: The number of segments.
*/
void print_mapping32(SectionToSegmentMapping *mapping, int num_segments)
{
	int i = 0;
	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	for (i = 0; i < num_segments; i++)
	{
		if (strlen(mapping[i].sections) > 0)
		{
			printf("   %02d     %s \n", mapping[i].segment_number, mapping[i].sections);
		}
		else
		{
			printf("   %02d     \n", mapping[i].segment_number);
		}
	}
}
