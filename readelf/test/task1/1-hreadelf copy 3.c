#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "hreadelf.h"

int main(int argc, char *argv[])
{
	FILE *file = NULL;

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

	for (int i = 0; i < elf_header.e_shnum; i++)
	{
		fread(&section_header, sizeof(Elf32_Shdr), 1, file);
		/* imprimir la información*/
		const char *flags = getSectionFlags(section_header.sh_flags);
		printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %s %2d %2d %2d \n",
		i, "Guenoel", getSectionTypeName(section_header.sh_type),
		section_header.sh_addr, section_header.sh_offset,
		section_header.sh_size, section_header.sh_entsize, flags,
		section_header.sh_link, section_header.sh_info, section_header.sh_addralign);
	}

	fclose(file);
	return 0;
}

const char *getSectionFlags(unsigned int sh_flags)
{
	static char flags[4];
	flags[0] = (sh_flags & SHF_WRITE) ? 'W' : ' ';
	flags[1] = (sh_flags & SHF_ALLOC) ? 'A' : ' ';
	flags[2] = (sh_flags & SHF_EXECINSTR) ? 'X' : ' ';
	flags[3] = '\0';
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
