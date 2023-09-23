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

	/* Leer la cabecera ELF */
	Elf32_Ehdr elf_header32;
	Elf64_Ehdr elf_header64;

	if (fread(&elf_header32, sizeof(Elf32_Ehdr), 1, file) != 1 &&
		fread(&elf_header64, sizeof(Elf64_Ehdr), 1, file) != 1)
	{
		perror("Error reading ELF header");
		fclose(file);
		return 1;
	}

	/* Determinar la clase ELF */
	uint8_t elf_class = ELFCLASSNONE;

	if (elf_header32.e_ident[EI_CLASS] == ELFCLASS32)
	{
		elf_class = ELFCLASS32;
	}
	else if (elf_header32.e_ident[EI_CLASS] == ELFCLASS64)
	{
		elf_class = ELFCLASS64;
	}
	else
	{
		fprintf(stderr, "Unsupported ELF class\n");
		fclose(file);
		return 1;
	}

	/* Leer la posición de la tabla de secciones */
	off_t section_table_offset = elf_class == ELFCLASS32 ? elf_header32.e_shoff : elf_header64.e_shoff;

	/*  Imprimir secciones y la posición de inicio*/
	printf("There are %u section headers, starting at offset 0x%llx:\n\n",
		elf_class == ELFCLASS32 ? elf_header32.e_shnum : elf_header64.e_shnum,
		(long long)section_table_offset);
	printf("Section Headers:\n");
	printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

	/* Read and print section headers based on the ELF format */
	if (elf_class == ELFCLASS32)
	{
		MyElf32_Shdr section_header;
		int section_number = 0;

		while (fread(&section_header, sizeof(MyElf32_Shdr), 1, file))
		{
			printElf32SectionHeader(&section_header, section_number);
			section_number++;
		}
	}
	else if (elf_class == ELFCLASS64)
	{
		MyElf64_Shdr section_header;
		int section_number = 0;

		while (fread(&section_header, sizeof(MyElf64_Shdr), 1, file))
		{
			printElf64SectionHeader(&section_header, section_number);
			section_number++;
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

void printElf32SectionHeader(const MyElf32_Shdr *section_header, int section_number)
{
printf("  [%2d] %18d %16s %08x %08x %08x %08x %2u %2u %2u %08x\n",
	section_number,
	section_header->sh_name,
	getSectionTypeName(section_header->sh_type),
	section_header->sh_addr,
	section_header->sh_offset,
	section_header->sh_size,
	section_header->sh_entsize,
	section_header->sh_flags,
	section_header->sh_link,
	section_header->sh_info,
	section_header->sh_addralign);
}

void printElf64SectionHeader(const MyElf64_Shdr *section_header, int section_number)
{
printf("  [%2d] %2u %-16s %-8s %016lx %016lx %016lx %2u %2lu %2lu %016x\n",
	section_number,	section_header->sh_name, "NULL", "NULL", section_header->sh_addr,
	section_header->sh_offset, section_header->sh_size,
	section_header->sh_type, section_header->sh_flags,
	section_header->sh_entsize, section_header->sh_link);
}


const char *getSectionTypeName(unsigned int sh_type)
{
    switch (sh_type)
    {
	case SHT_NULL:    return "NULL";
	case SHT_PROGBITS:    return "PROGBITS";
	case SHT_RELA:        return "RELA";
	case SHT_NOTE:        return "NOTE";
	case SHT_HASH:        return "HASH";
	case SHT_DYNSYM:    return "DYNSYM";
	case SHT_STRTAB:        return "STRAB";
	case SHT_REL:        return "REL";
	case SHT_NOBITS:        return "NOBITS";
	case SHT_SYMTAB:        return "SYMTAB";
	case SHT_INIT_ARRAY:        return "INI_ARRAY";
	case SHT_FINI_ARRAY:        return "FINI_ARRAY";
	case DT_VERSYM:        return "VERSYM";
	case DT_VERNEED:        return "VERNEED";
	case SHT_DYNAMIC:        return "DYNAMIC";
	default:               return "UNKNOWN";
    }
}
