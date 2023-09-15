#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "hreadelf.h"


int main(int argc, char *argv[])
{
	FILE *file = NULL;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
		return 1;
	}

	file = fopen(argv[1], "rb");
	if (!file)
	{
		perror("Error opening file");
		return 1;
	}

	/* Read the ELF header to determine the format (32-bit or 64-bit) */
	uint8_t elf_class;
	fseek(file, EI_CLASS, SEEK_SET);
	fread(&elf_class, sizeof(uint8_t), 1, file);

	/* Seek to the section header table offset in the ELF header */
	fseek(file, elf_class == ELFCLASS32 ? 0x20 : 0x28, SEEK_SET);

	/* Read and print section headers based on the ELF format */
	if (elf_class == ELFCLASS32)
	{
		MyElf32_Shdr section_header;
		printf("There's a 32-bit ELF file.\n");

		while (fread(&section_header, sizeof(MyElf32_Shdr), 1, file))
		{
			/* Print todo */
			printElf32SectionHeader(&section_header);
		}
	}
	else if (elf_class == ELFCLASS64)
	{
		MyElf64_Shdr section_header;
		printf("There's a 64-bit ELF file.\n");

		while (fread(&section_header, sizeof(MyElf64_Shdr), 1, file))
		{
			/* Print todo */
			printElf64SectionHeader(&section_header);
		}
	}
	else
	{
		fprintf(stderr, "Unsupported ELF class\n");
		fclose(file);
		return 1;
	}
	fclose(file);
	return 0;
}

void printElf32SectionHeader(const MyElf32_Shdr *section_header)
{
printf("  [ %2u] %-16s %-8s %08x %08x %08x %2u %2u %2u %08x\n",
	section_header->sh_name, "NULL", "NULL", section_header->sh_addr,
	section_header->sh_offset, section_header->sh_size,
	section_header->sh_type, section_header->sh_flags,
	section_header->sh_entsize, section_header->sh_link);
}

void printElf64SectionHeader(const MyElf64_Shdr *section_header)
{
printf("  [ %2u] %-16s %-8s %016lx %016lx %016lx %2u %2lu %2lu %016x\n",
	section_header->sh_name, "NULL", "NULL", section_header->sh_addr,
	section_header->sh_offset, section_header->sh_size,
	section_header->sh_type, section_header->sh_flags,
	section_header->sh_entsize, section_header->sh_link);
}
