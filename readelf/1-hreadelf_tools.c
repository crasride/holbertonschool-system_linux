#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "hreadelf.h"

char *get_section_name32(Elf32_Shdr section_header, FILE *file)
{
	char *SectNames = NULL;

	fread(&section_header, 1, sizeof(section_header), file);
	SectNames = (char *)malloc(section_header.sh_size);
	fseek(file, section_header.sh_offset, SEEK_SET);
	fread(SectNames, 1, section_header.sh_size, file);
	return (SectNames);
}

char *get_section_name32_big(Elf32_Shdr section_header, FILE *file)
{
	char *SectNames = NULL;

		fread(&section_header, 1, sizeof(section_header), file);
		read_elf32_be_section(&section_header);
		SectNames = (char *)malloc(section_header.sh_size);
		fseek(file, section_header.sh_offset, SEEK_SET);
		fread(SectNames, 1, section_header.sh_size, file);
	return (SectNames);
}


char *get_section_name64(Elf64_Shdr section_header, FILE *file)
{
	char *SectNames = NULL;

	fread(&section_header, 1, sizeof(section_header), file);
	SectNames = (char *)malloc(section_header.sh_size);
	fseek(file, section_header.sh_offset, SEEK_SET);
	fread(SectNames, 1, section_header.sh_size, file);
	return (SectNames);
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

	if (sh_flags & SHF_STRINGS)
		flags[i--] = 'S';

	if (sh_flags & SHF_INFO_LINK)
		flags[i--] = 'I';

	if (sh_flags & SHF_MERGE)
		flags[i--] = 'M';

	if (sh_flags & SHF_EXECINSTR)
		flags[i--] = 'X';

	if (sh_flags & SHF_ALLOC)
		flags[i--] = 'A';

	if (sh_flags & SHF_WRITE)
		flags[i--] = 'W';

	if (sh_flags & SHF_EXCLUDE)
		flags[i--] = 'E';

	return (flags);
}

const char *getSectionTypeName(unsigned int sh_type)
{
	switch (sh_type)
	{
	case SHT_NULL:         return "NULL";
	case SHT_PROGBITS:     return "PROGBITS";
	case SHT_RELA:         return "RELA";
	case SHT_NOTE:         return "NOTE";
	case SHT_HASH:         return "HASH";
	case SHT_DYNSYM:       return "DYNSYM";
	case SHT_STRTAB:       return "STRTAB";
	case SHT_REL:          return "REL";
	case SHT_NOBITS:       return "NOBITS";
	case SHT_SYMTAB:       return "SYMTAB";
	case SHT_INIT_ARRAY:   return "INIT_ARRAY";
	case SHT_FINI_ARRAY:   return "FINI_ARRAY";
	case DT_VERSYM:        return "VERSYM";
	case SHT_GNU_versym:   return "VERSYM";
	case DT_VERNEED:       return "VERNEED";
	case SHT_DYNAMIC:      return "DYNAMIC";
	case SHT_GNU_HASH:     return "GNU_HASH";
	case DT_VERDEF:        return "VERDEF";
	case SHT_LOOS + 0xffffff3: return "LOOS+ffffff3";
	case SHT_LOOS + 0xffffff1: return "LOOS+ffffff1";
	default:               return "UNKNOWN";
	}
}
