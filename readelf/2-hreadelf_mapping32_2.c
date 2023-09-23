#include "hreadelf.h"


/**
 * sections_selection - Select sections relevant to a program header.
 *
 * This function determines and selects sections from the ELF file are relevant
 * to a given program header. It iterates through section headers, checks their
 * addresses, and compiles list of section names that fall within the program's
 * address range. It then stores selected section names the 'sections' field
 * of the 'SectionToSegmentMapping' structure at index 'i' the 'mapping' array.
 *
 * @elf_header:  A pointer the ElfHeader structcontaining ELF header info.
 * @section_headers: An array Elf32_Shdr struct representing section headers.
 * @program_header: The program header for which sections are being selected.
 * @shstrtab:       A pointer to the section header string table.
 * @mapping:        An array of SectionToSegmentMapping structures.
 * @i:     The index in the 'mapping' array where section names will be stored.
 */
void sections_selection(ElfHeader *elf_header, Elf32_Shdr *section_headers,
						Elf32_Phdr program_header, char *shstrtab,
						SectionToSegmentMapping *mapping, int i)
{
	char sections[MAX_INTERP_SIZE] = "";
	int j = 0;

	strcpy(sections, "");
	for (j = 0; j < elf_header->ehdr.ehdr32.e_shnum; j++)
	{
		const char *section_name;
		Elf32_Shdr section_header = section_headers[j];

		if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
			read_elf32_be_section(&section_header);
		section_name = shstrtab + section_header.sh_name;
		if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2LSB)
		{
			if (strcmp(section_name, ".gnu_debuglink") == 0 ||
				strcmp(section_name, ".shstrtab") == 0 ||
				strcmp(section_name, ".tm_clone_table") == 0)
				continue;
		}
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
