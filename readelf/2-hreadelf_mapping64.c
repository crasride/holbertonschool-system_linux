#include "hreadelf.h"





/**
* createSectionToSegmentMapping64 - Create a mapping between sections and
* segments in a 64-bit ELF file.
* This function creates a mapping between sections and segments in the provided
* ELF file
* for 64-bit ELF files. It calculates which sections belong to each segment
* and prints the mapping information.
*
* @file:            A pointer to the open ELF file.
* @elf_header: A pointer to the ElfHeader structure containing ELF header inf
* @is_32bit: flag the ELF file is 32-bit (if true) or 64-bit (if false).
*/
void createSectionToSegmentMapping64(FILE *file, ElfHeader *elf_header,
									int is_32bit)
{
	int i = 0;
	Elf64_Shdr section_headers[MAX_INTERP_SIZE];
	SectionToSegmentMapping *mapping =
	(SectionToSegmentMapping *)malloc(elf_header->
	ehdr.ehdr64.e_phnum * sizeof(SectionToSegmentMapping));

	for (i = 0; i < elf_header->ehdr.ehdr64.e_phnum; i++)
	{
		mapping[i].segment_number = i;
		strcpy(mapping[i].sections, "");
	}

	calculateSectionsInSegment64(file, elf_header, is_32bit, section_headers,
								mapping);
	print_mapping64(mapping, elf_header->ehdr.ehdr64.e_phnum);
	free(mapping);
}

/**
* calculateSectionsInSegment64 - Calculate sections belonging to each program
* segment in a 64-bit ELF file.
* This function calculates the sections that belong to each program segment
* and populates the `sections` field of the `mapping` array with the
* corresponding section names.
* @file:             A pointer to the open ELF file.
* @elf_header:pointer to ElfHeader structure containing ELF header information.
* @is_32bit:  A flag the ELF file is 32-bit (if true) or 64-bit (if false).
* @section_headers: array of Elf64_Shdr structures representing section headers
* @mapping:     An array of SectionToSegmentMapping structures to be populated.
*/
void calculateSectionsInSegment64(FILE *file, ElfHeader *elf_header,
int is_32bit, Elf64_Shdr *section_headers, SectionToSegmentMapping *mapping)
{
	int i, j;
	Elf64_Shdr shstrtab_header;
	char *shstrtab;
	char sections[MAX_INTERP_SIZE] = "";

	readSectionHeaders(file, section_headers, elf_header, is_32bit);
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->
			ehdr.ehdr64.e_shoff) + elf_header->
			ehdr.ehdr64.e_shstrndx * sizeof(Elf64_Shdr), SEEK_SET);
	fread(&shstrtab_header, sizeof(Elf64_Shdr), 1, file);
	shstrtab = getSectionNameTable(file, shstrtab_header);
	fseek(file, elf_header->ehdr.ehdr64.e_phoff, SEEK_SET);
	for (i = 0; i < elf_header->ehdr.ehdr64.e_phnum; i++)
	{
		Elf64_Phdr program_header;

		fread(&program_header, sizeof(Elf64_Phdr), 1, file);
		strcpy(sections, "");
		for (j = 0; j < elf_header->ehdr.ehdr64.e_shnum; j++)
		{
			Elf64_Shdr section_header = section_headers[j];
			const char *section_name = shstrtab + section_header.sh_name;
				if (strcmp(section_name, ".gnu_debuglink") == 0 ||
					strcmp(section_name, ".shstrtab") == 0 ||
					strcmp(section_name, ".tm_clone_table") == 0)
					continue;
			if (section_header.sh_addr >= program_header.p_vaddr &&
				section_header.sh_addr + section_header.sh_size <=
				program_header.p_vaddr + program_header.p_memsz)
			{
				if (strlen(sections) > 0)
					strcat(sections, " ");
				strcat(sections, section_name);
			}
		}
		if (strlen(sections) > 0)
			strcpy(mapping[i].sections, sections);
	}
	free(shstrtab);
}

/**
* readSectionHeaders - Read section headers from an ELF file and store them in
* an array.
* This function reads the section headers from the provided ELF file and stores
* in the given array of Elf64_Shdr structures.
*
* @file:A pointer to the open ELF file.
* @section_headers: An array Elf64_Shdr structures to store the section headers
* @elf_header:pointer to ElfHeader structure containing ELF header information.
* @is_32bit:  A flag the ELF file is 32-bit (if true) or 64-bit (if false).
*/
void readSectionHeaders(FILE *file, Elf64_Shdr *section_headers,
						ElfHeader *elf_header, int is_32bit)
{
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff :
		elf_header->ehdr.ehdr64.e_shoff), SEEK_SET);
	fread(section_headers, sizeof(Elf64_Shdr), elf_header->ehdr.ehdr64.e_shnum,
		 file);
}

/**
* getSectionNameTable - Read and retrieve section name table from an ELF file.
*
* This function reads section name table from the provided ELF file and returns
* it as a character string.
*
* @file: A pointer to the open ELF file.
* @shstrtab_header: The header of the section that contains the section names.
*
* Return: A dynamically allocated character string containing section names.
* The caller is responsible for freeing the memory when done.
*/
char *getSectionNameTable(FILE *file, Elf64_Shdr shstrtab_header)
{
	char *shstrtab = (char *)malloc(shstrtab_header.sh_size);

	fseek(file, shstrtab_header.sh_offset, SEEK_SET);
	fread(shstrtab, shstrtab_header.sh_size, 1, file);
	return (shstrtab);
}


/**
* print_mapping64 - Print the mapping of sections to segments in an ELF file
*
* This function takes an array of SectionToSegmentMapping structures and number
* of segments as input and prints the mapping in a human-readable format to
* the standard output.
* @mapping: An array of SectionToSegmentMapping structures.
* @num_segments: The number of segments.
*/
void print_mapping64(SectionToSegmentMapping *mapping, int num_segments)
{
	int i = 0;

	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	for (i = 0; i < num_segments; i++)
	{
		if (strlen(mapping[i].sections) > 0)
		{
			printf("   %02d     %s \n", mapping[i].segment_number,
					mapping[i].sections);
		}
		else
		{
			printf("   %02d     \n", mapping[i].segment_number);
		}
	}
}
