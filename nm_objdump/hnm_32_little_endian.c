#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>
#include "hnm.h"


const char *get_symbol_type_32(uint8_t info, Elf32_Sym sym, Elf32_Shdr *shdr)
{
	if (ELF32_ST_BIND(info) == STB_GNU_UNIQUE)
		return ("u");
	if (ELF32_ST_BIND(info) == STB_WEAK && ELF32_ST_TYPE(info) == STT_OBJECT)
		return ((sym.st_shndx == SHN_UNDEF) ? "v" : "V");
	if (ELF32_ST_BIND(info) == STB_WEAK)
		return ((sym.st_shndx == SHN_UNDEF) ? "w" : "W");
	else if (sym.st_shndx == SHN_UNDEF)
		return ("U");
	else if (sym.st_shndx == SHN_ABS)
		return ("A");
	else if (sym.st_shndx == SHN_COMMON)
		return ("C");
	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		return ("B");
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
		return ("R");
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		return ("D");
	if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
		shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		return ((ELF32_ST_BIND(info) == STB_GLOBAL) ? "T" : "t");
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		return ("D");
	else
		return ("T");
}

void process_symbols_32bit(Elf32_Ehdr *ehdr, void *map, const char *filename)
{
	int i, num_symbols;
	Elf32_Sym *symtab;
	char *strtab_data;
	Elf32_Shdr *shdr = (Elf32_Shdr *)((char *)map + ehdr->e_shoff);
	Elf32_Shdr *symtab_section = NULL;
	Elf32_Shdr *strtab_section = NULL;

	for (i = 0; i < ehdr->e_shnum; i++)
	{
		if (shdr[i].sh_type == SHT_SYMTAB)
			symtab_section = &shdr[i];
		else if (shdr[i].sh_type == SHT_STRTAB)
			strtab_section = &shdr[i];
	}
	if (!symtab_section || !strtab_section)
	{
		fprintf(stderr, "./hnm: %s: no symbols\n", filename);
		return;
	}
	/* Acceso tabla de símbolos y tabla de str */
	symtab = (Elf32_Sym *)((char *)map + symtab_section->sh_offset);
	strtab_data = (char *)((char *)map + strtab_section->sh_offset);
	num_symbols = symtab_section->sh_size / sizeof(Elf32_Sym);
	/* Recorre los símbolos y muestra la información */
	for (i = 0; i < num_symbols; i++)
	{
		if ((symtab[i].st_name != 0) && strcmp(strtab_data + symtab[i].st_name,
		"main.c") != 0)
		{
			char *symbol_name = strtab_data + symtab[i].st_name;
			const char *symbol_type_str = get_symbol_type_32(symtab[i].st_info,
			symtab[i], shdr);

			if (symbol_type_str[0] != 'U')
				printf("%08x %s %s\n", symtab[i].st_value, symbol_type_str,
						symbol_name);
			else
				printf("         %s %s\n", symbol_type_str, symbol_name);
		}
	}
}

int analyze_32bit_elf(Elf32_Ehdr *ehdr, void *map, const char *filename)
{

	/* printf("32 bits, "); */
	if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
	{
		/* printf("little-endian.\n"); */
		process_symbols_32bit(ehdr, map, filename);
	}
	else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		process_symbols_32bit_big_endian(ehdr, map, filename);
	}
	else
	{
		printf("endianness unknown.\n");
	}

	return (0);
}
