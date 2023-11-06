#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>
#include "hnm.h"


const char *get_symbol_type_64(uint8_t info, Elf64_Sym sym, Elf64_Shdr *shdr)
{
	if (ELF64_ST_BIND(info) == STB_GNU_UNIQUE)
		return ("u");
	if (ELF64_ST_BIND(info) == STB_WEAK && ELF64_ST_TYPE(info) == STT_OBJECT)
		return ((sym.st_shndx == SHN_UNDEF) ? "v" : "V");
	if (ELF64_ST_BIND(info) == STB_WEAK)
		return ((sym.st_shndx == SHN_UNDEF) ? "w" : "W");
	else if (sym.st_shndx == SHN_UNDEF)
		return ("U");
	else if (sym.st_shndx == SHN_ABS)
		return ("A");
	else if (sym.st_shndx == SHN_COMMON)
		return ("C");
	else if (shdr[sym.st_shndx].sh_type == SHT_NOBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		return ((ELF64_ST_BIND(info) == STB_GLOBAL) ? "B" : "b");
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == SHF_ALLOC)
		return ((ELF64_ST_BIND(info) == STB_GLOBAL) ? "R" : "r");
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
	return ((ELF64_ST_BIND(info) == STB_GLOBAL) ? "D" : "d");
	else if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS &&
			shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
	return ((ELF64_ST_BIND(info) == STB_GLOBAL) ? "T" : "t");
	else if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC)
		return ("d");
	else
		return ("t");
}

void process_symbols_64bit(Elf64_Ehdr *ehdr, void *map, const char *filename)
{
	int i, num_symbols;
	Elf64_Sym *symtab;
	char *strtab_data;
	Elf64_Shdr *shdr = (Elf64_Shdr *)((char *)map + ehdr->e_shoff);

	/* busca seccion (SHT_SYMTAB) y la tabla de cadenas asociada (SHT_STRTAB) */
	Elf64_Shdr *symtab_section = NULL;
	Elf64_Shdr *strtab_section = NULL;

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
	symtab = (Elf64_Sym *)((char *)map + symtab_section->sh_offset);
	strtab_data = (char *)((char *)map + strtab_section->sh_offset);

	num_symbols = symtab_section->sh_size / sizeof(Elf64_Sym);

	/* Recorre los símbolos y muestra la información */
	for (i = 0; i < num_symbols; i++)
	{
		/* Verifica si el nombre del símbolo contiene la extensión ".c" y omite la impresión */
		char *symbol_name = strtab_data + symtab[i].st_name;
		if (strstr(symbol_name, ".c") != NULL)
			continue;

		if (symtab[i].st_name)
		{
			const char *symbol_type_str = get_symbol_type_64(symtab[i].st_info, symtab[i], shdr);

			if (symbol_type_str[0] != 'U' && symbol_type_str[0] != 'w')
				printf("%16.16lx %s %s\n", symtab[i].st_value, symbol_type_str, symbol_name);
			else
				printf("                 %s %s\n", symbol_type_str, symbol_name);
		}
	}
}

int analyze_64bit_elf(Elf64_Ehdr *ehdr, void *map, const char *filename)
{
	if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
	{
		/* printf("little-endian.\n"); */
		process_symbols_64bit(ehdr, map, filename);

	}
	else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		printf("big-endian.\n");
	}
	else
	{
		printf("endianness unknown.\n");
	}

	return (0);
}
