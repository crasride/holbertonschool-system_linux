#include "hreadelf.h"

/**
* createSectionToSegmentMapping32 - Create a mapping between sections and
* segments in a 32-bit ELF file.
* This function creates a mapping between sections and segments in the provided
* ELF file
* for 32-bit ELF files. It calculates which sections belong to each segment
* and prints the mapping information.
*
* @file:            A pointer to the open ELF file.
* @elf_header: A pointer to the ElfHeader structure containing ELF header inf
* @is_32bit: flag the ELF file is 32-bit (if true) or 32-bit (if false).
*/
void createSectionToSegmentMapping32(FILE *file, ElfHeader *elf_header,
									int is_32bit)
{
	int i = 0;
	Elf32_Shdr section_headers[MAX_INTERP_SIZE];
	SectionToSegmentMapping *mapping =
	(SectionToSegmentMapping *)malloc(elf_header->
	ehdr.ehdr32.e_phnum * sizeof(SectionToSegmentMapping));

	for (i = 0; i < elf_header->ehdr.ehdr32.e_phnum; i++)
	{
		mapping[i].segment_number = i;
		strcpy(mapping[i].sections, "");
	}

	calculateSectionsInSegment32(file, elf_header, is_32bit, section_headers,
								mapping);
	print_mapping32(mapping, elf_header->ehdr.ehdr32.e_phnum);
	free(mapping);
}

/**
* calculateSectionsInSegment32 - Calculate sections belonging to each program
* segment in a 32-bit ELF file.
* This function calculates the sections that belong to each program segment
* and populates the `sections` field of the `mapping` array with the
* corresponding section names.
* @file:             A pointer to the open ELF file.
* @elf_header:pointer to ElfHeader structure containing ELF header information.
* @is_32bit:  A flag the ELF file is 32-bit (if true) or 32-bit (if false).
* @section_headers: array of Elf32_Shdr structures representing section headers
* @mapping:     An array of SectionToSegmentMapping structures to be populated.
*/
void calculateSectionsInSegment32(FILE *file, ElfHeader *elf_header,
int is_32bit, Elf32_Shdr *section_headers, SectionToSegmentMapping *mapping)
{
	int i;
	Elf32_Shdr shstrtab_header;
	char *shstrtab;

	readSectionHeaders32(file, section_headers, elf_header, is_32bit);
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->
			ehdr.ehdr32.e_shoff) + elf_header->
			ehdr.ehdr32.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
	fread(&shstrtab_header, sizeof(Elf32_Shdr), 1, file);
	if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
		read_elf32_be_section(&shstrtab_header);
	shstrtab = getSectionNameTable32(file, shstrtab_header);
	fseek(file, elf_header->ehdr.ehdr32.e_phoff, SEEK_SET);
	for (i = 0; i < elf_header->ehdr.ehdr32.e_phnum; i++)
	{
		Elf32_Phdr program_header;

		fread(&program_header, sizeof(Elf32_Phdr), 1, file);
		if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
			read_elf32_be_prog(&program_header);
		sections_selection(elf_header, section_headers, program_header,
							shstrtab, mapping, i);
	}
	free(shstrtab);
}

/**
* readSectionHeaders - Read section headers from an ELF file and store them in
* an array.
* This function reads the section headers from the provided ELF file and stores
* in the given array of Elf32_Shdr structures.
*
* @file:A pointer to the open ELF file.
* @section_headers: An array Elf32_Shdr structures to store the section headers
* @elf_header:pointer to ElfHeader structure containing ELF header information.
* @is_32bit:  A flag the ELF file is 32-bit (if true) or 32-bit (if false).
*/
void readSectionHeaders32(FILE *file, Elf32_Shdr *section_headers,
						ElfHeader *elf_header, int is_32bit)
{
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff :
		elf_header->ehdr.ehdr32.e_shoff), SEEK_SET);
	fread(section_headers, sizeof(Elf32_Shdr), elf_header->ehdr.ehdr32.e_shnum,
		 file);
}

/**
* getSectionNameTable32 - Read and retrieve section name table from ELF file.
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
char *getSectionNameTable32(FILE *file, Elf32_Shdr shstrtab_header)
{
	char *shstrtab = (char *)malloc(shstrtab_header.sh_size);

	fseek(file, shstrtab_header.sh_offset, SEEK_SET);
	fread(shstrtab, shstrtab_header.sh_size, 1, file);
	return (shstrtab);
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
			printf("   %02d     %s \n", mapping[i].segment_number,
					mapping[i].sections);
		}
		else
		{
			printf("   %02d     \n", mapping[i].segment_number);
		}
	}
}

