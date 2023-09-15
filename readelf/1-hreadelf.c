#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "hreadelf.h"

int main(int argc, char *argv[])
{
	FILE *file = NULL;
	int index = 0;

	if (argc != 2) {
		fprintf(stderr, "Uso: %s <archivo ELF>\n", argv[0]);
		return 1;
	}
	file = fopen(argv[1], "rb");
	if (file == NULL) {
		perror("No se puede abrir el archivo");
		return 1;
	}

	/* Leer el encabezado ELF principal */
	Elf32_Ehdr elf_header;
	fread(&elf_header, sizeof(Elf32_Ehdr), 1, file);

	/* Ubicar la tabla de secciones */
	fseek(file, elf_header.e_shoff, SEEK_SET);

	/* Leer cada entrada de encabezado de sección */
	Elf32_Shdr section_header;
	printf("Section Headers:\n");
	printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");

	for (index = 0; index < elf_header.e_shnum; index++)
	{
		fread(&section_header, sizeof(Elf32_Shdr), 1, file);
		/* Llamar a la función para imprimir la información de la sección */
		print_Section_Info_32bits(index, section_header);
	}
	fclose(file);
	return 0;
}

void print_Section_Info_32bits(int index, Elf32_Shdr section_header)
{
	const char *flags = getSectionFlags(section_header.sh_flags);
	printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %s %2d %2d %2d \n",
		index, "Guenoel", getSectionTypeName(section_header.sh_type),
		section_header.sh_addr, section_header.sh_offset,
		section_header.sh_size, section_header.sh_entsize, flags,
		section_header.sh_link, section_header.sh_info, section_header.sh_addralign);
}

const char *getSectionFlags(unsigned int sh_flags)
{
	int i;
	static char flags[4];
	/* initalisation */
	for (i = 0; i < 3; i++)
		flags[i] = ' ';
	/* fill each character from the end with conditions */
	flags[3] = '\0';
	i = 2;
	if (sh_flags & SHF_MERGE)
	{
		flags[i] = 'M';
		i--;
	}
	if (sh_flags & SHF_EXECINSTR)
	{
		flags[i] = 'X';
		i--;
	}
	if (sh_flags & SHF_ALLOC)
	{
		flags[i] = 'A';
		i--;
	}
	if (sh_flags & SHF_INFO_LINK)
	{
		flags[i] = 'I';
		i--;
	}
	if (sh_flags & SHF_WRITE)
	{
		flags[i] = 'W';
		i--;
	}
	return flags;
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
	case SHT_STRTAB:        return "STRTAB";
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
