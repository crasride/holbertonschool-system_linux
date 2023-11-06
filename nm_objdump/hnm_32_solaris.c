#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>
#include "hnm.h"

void process_symbols_32bit_solaris(Elf32_Ehdr *ehdr, void *map, const char *filename)
{
		int i;
		Elf32_Sym *symtab;
		char *strtab_data;
		int num_symbols;
		Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)map + ehdr->e_shoff);

		Elf32_Shdr *symtab_section = NULL;
		Elf32_Shdr *strtab_section = NULL;

		for (i = 0; i < ehdr->e_shnum; i++)
		{
			if (shdr[i].sh_type == SHT_SYMTAB)
			{
				symtab_section = &shdr[i];
			}
			else if (shdr[i].sh_type == SHT_STRTAB)
			{
				strtab_section = &shdr[i];
			}
		}

		if (!symtab_section || !strtab_section)
		{
			fprintf(stderr, "./hnm: %s: no symbols\n", filename);
			return;
		}

		symtab = (Elf32_Sym *)((char *)map + symtab_section->sh_offset);
		strtab_data = (char *)((char *)map + strtab_section->sh_offset);

		num_symbols = symtab_section->sh_size / sizeof(Elf32_Sym);

		for (i = 0; i < num_symbols; i++)
		{
			if (symtab[i].st_name != 0)
			{
				char *symbol_name = strtab_data + symtab[i].st_name;
				printf("%08x %s\n", symtab[i].st_value, symbol_name);
			}
		}
	}


int analyze_32bit_elf_solaris(Elf32_Ehdr *ehdr, void *map, const char *filename)
{
	if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
	{
		process_symbols_32bit_solaris(ehdr, map, filename);
	}
	else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		printf("big endian Solaris.\n");
	}
	else
	{
		printf("endianness unknown.\n");
	}
	return (0);
}
