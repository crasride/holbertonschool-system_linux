#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "hreadelf.h"

int main(int argc, char *argv[])
{
	FILE *file = NULL;
	int index = 0;
	Elf32_Ehdr elf_header32;
	Elf64_Ehdr elf_header64;
	off_t section_table_offset;
	Elf32_Shdr section_header32;
	Elf64_Shdr section_header64;
	char* SectNames = NULL;
	int is_32bit = 0; /* Variable para detectar si es un archivo de 32 bits */
	if (argc != 2) {
		return (EXIT_SUCCESS);
	}
	file = fopen(argv[1], "rb");
	if (file == NULL) {
		perror("No se puede abrir el archivo");
		return 1;
	}
	/* Leer el encabezado ELF principal */
	fread(&elf_header32, sizeof(Elf32_Ehdr), 1, file);
	/* Verificar si es un archivo ELF de 32 bits */
	if (elf_header32.e_ident[EI_CLASS] == ELFCLASS32)
	{
		is_32bit = 1;
	}
	else
	{
		/* Retroceder al principio del archivo si no es un archivo de 32 bits */
		fseek(file, 0, SEEK_SET);
		/* Leer el encabezado ELF de 64 bits */
		fread(&elf_header64, sizeof(Elf64_Ehdr), 1, file);
	}
	/* Leer la posición de la tabla de secciones */
	section_table_offset = is_32bit ? elf_header32.e_shoff : elf_header64.e_shoff;
	/* Create sectHdr and SectNames */
	if (is_32bit)
	{
		fseek(file, elf_header32.e_shoff + elf_header32.e_shstrndx * elf_header32.e_shentsize, SEEK_SET);
		SectNames = get_section_name32(section_header32, file);
	}
	else
	{
		fseek(file, elf_header64.e_shoff + elf_header64.e_shstrndx * elf_header64.e_shentsize, SEEK_SET);
		SectNames = get_section_name64(section_header64, file);
	}
	/* Imprimir la información sobre la tabla de secciones */
	printf("There are %u section headers, starting at offset 0x%lx:\n\n",
		is_32bit ? elf_header32.e_shnum : elf_header64.e_shnum,
		section_table_offset);
	/* Ubicar la tabla de secciones */
	if (is_32bit) {
		fseek(file, elf_header32.e_shoff, SEEK_SET);
	}
	else {
		fseek(file, elf_header64.e_shoff, SEEK_SET);
	}
	/* Leer cada entrada de encabezado de sección */
	printf("Section Headers:\n");
	printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
	if (is_32bit) {
		Elf32_Shdr section_header32;
		for (index = 0; index < elf_header32.e_shnum; index++)
		{
			char* name = "";
			fread(&section_header32, sizeof(Elf32_Shdr), 1, file);

			if (section_header32.sh_name)
				name = SectNames + section_header32.sh_name;
			/* Llamar a la función para imprimir la información de la sección de 32 bits */
			print_Section_Info_32bits(index, section_header32, name);
		}
	}
	else
	{
		Elf64_Shdr section_header64;
		for (index = 0; index < elf_header64.e_shnum; index++)
		{
			char* name = "";
			fread(&section_header64, sizeof(Elf64_Shdr), 1, file);

			if (section_header64.sh_name)
				name = SectNames + section_header64.sh_name;
			/* Llamar a la función para imprimir la información de la sección de 64 bits */
			print_Section_Info_64bits(index, section_header64, name);
		}
	}
	printKeyToFlags();
	fclose(file);
	return 0;
}


void printKeyToFlags()
{
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
}
void print_Section_Info_32bits(int index, Elf32_Shdr section_header, char *name)
{
	const char *flags = getSectionFlags(section_header.sh_flags);
	printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %s %2d %3d %2d \n",
		index, name, getSectionTypeName(section_header.sh_type),
		section_header.sh_addr, section_header.sh_offset,
		section_header.sh_size, section_header.sh_entsize, flags,
		section_header.sh_link, section_header.sh_info, section_header.sh_addralign);
}
void print_Section_Info_64bits(int index, Elf64_Shdr section_header, char *name)
{
	const char *flags = getSectionFlags(section_header.sh_flags);
	printf("  [%2d] %-17s %-15s %08lx %06lx %06lx %02lx %s %2d %3d %2ld \n",
		index, name, getSectionTypeName(section_header.sh_type),
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
	if (sh_flags & SHF_STRINGS)
	{
		flags[i] = 'S';
		i--;
	}
	if (sh_flags & SHF_INFO_LINK)
	{
		flags[i] = 'I';
		i--;
	}
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
    case SHT_INIT_ARRAY:   return "INI_ARRAY";
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

char *get_section_name32(Elf32_Shdr section_header, FILE *file)
{
	char* SectNames = NULL;
	fread(&section_header, 1, sizeof(section_header), file);
	SectNames = (char *)malloc(section_header.sh_size);
	fseek(file, section_header.sh_offset, SEEK_SET);
	fread(SectNames, 1, section_header.sh_size, file);
	return (SectNames);
}
char *get_section_name64(Elf64_Shdr section_header, FILE *file)
{
	char* SectNames = NULL;
	fread(&section_header, 1, sizeof(section_header), file);
	SectNames = (char *)malloc(section_header.sh_size);
	fseek(file, section_header.sh_offset, SEEK_SET);
	fread(SectNames, 1, section_header.sh_size, file);
	return (SectNames);
}
