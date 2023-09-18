#include "hreadelf.h"


/**
* get_section_name32 - Retrieve the name of an ELF32 section little endian.
* This function reads and returns the name of an ELF32 section based on the
* provided section headerand file pointer.
* @section_header: The ELF32 section header structure.
* @file: A pointer to the ELF file.
* Return: A dynamically allocated string containing the section name.
* The caller is responsible for freeing the allocated memory.
*/
char *get_section_name32(Elf32_Shdr section_header, FILE *file)
{
	char *section_names = NULL;

	fread(&section_header, 1, sizeof(section_header), file);
	section_names = (char *)malloc(section_header.sh_size);
	fseek(file, section_header.sh_offset, SEEK_SET);
	fread(section_names, 1, section_header.sh_size, file);
	return (section_names);
}

/**
* get_section_name32_big - Retrieve the name of an ELF32 section big endian.
* This function reads and returns the name of an ELF32 section based on the
* provided section headerand file pointer.
* @section_header: The ELF32 section header structure.
* @file: A pointer to the ELF file.
* Return: A dynamically allocated string containing the section name.
* The caller is responsible for freeing the allocated memory.
*/
char *get_section_name32_big(Elf32_Shdr section_header, FILE *file)
{
	char *section_names = NULL;

		fread(&section_header, 1, sizeof(section_header), file);
		read_elf32_be_section(&section_header);
		section_names = (char *)malloc(section_header.sh_size);
		fseek(file, section_header.sh_offset, SEEK_SET);
		fread(section_names, 1, section_header.sh_size, file);
	return (section_names);
}

/**
* get_section_name64 - Retrieve the name of an ELF64 section little endian.
* This function reads and returns the name of an ELF64 section based on the
* provided section headerand file pointer.
* @section_header: The ELF64 section header structure.
* @file: A pointer to the ELF file.
* Return: A dynamically allocated string containing the section name.
* The caller is responsible for freeing the allocated memory.
*/
char *get_section_name64(Elf64_Shdr section_header, FILE *file)
{
	char *section_names = NULL;

	fread(&section_header, 1, sizeof(section_header), file);
	section_names = (char *)malloc(section_header.sh_size);
	fseek(file, section_header.sh_offset, SEEK_SET);
	fread(section_names, 1, section_header.sh_size, file);
	return (section_names);
}

/**
* getSectionFlags - Convert ELF section flags to a string representation.
* This function converts the ELF section flags into a string representation.
* Each flag character represents a specific attribute, such as Write (W),
* Alloc (A),
* Execute (X), Merge (M), Strings (S), Info Link (I), and Exclude (E).
* Flags not present are represented by spaces.
* @sh_flags: The ELF section flags to be converted.
* Return: A string representation of the section flags.
*/
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

/**
* getSectionTypeName - Convert an ELF section type to a string representation.
* This function converts the ELF section type into a string representation.
* It returns human-readable names for known section types such as PROGBITS,
* RELA,NOTE, HASH, DYNSYM, STRTAB, and others. For unknown types, it returns
* "UNKNOWN".
* @sh_type: The ELF section type to be converted.
* Return: A string representation of the section type.
*/
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
