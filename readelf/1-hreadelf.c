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

	if (argc != 2)
	{
		return (EXIT_SUCCESS);
	}
	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		perror("No se puede abrir el archivo");
		return (1);
	}
	/* Leer el encabezado ELF principal */
	fread(&elf_header32, sizeof(Elf32_Ehdr), 1, file);
	/* Verificar si es un archivo ELF de 32 bits */
	if (elf_header32.e_ident[EI_CLASS] == ELFCLASS32)
	{
		is_32bit = 1;

		/* Detectar el endianness del archivo ELF de 32 bits */
		if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
		{
			{
				read_elf32_be_header(&elf_header32);
			}
			/* Si es big-endian, usa la función para leer el encabezado */
		} else if (elf_header32.e_ident[EI_DATA] == ELFDATA2LSB)
		{
			/* Si es little-endian, puedes seguir con el código actual. */
		} else
		{
			printf("Formato de datos ELF de 32 bits no reconocido.\n");
			return (1);
		}
	}
	else
	{
		/* Retroceder al principio del archivo si no es un archivo de 32 bits */
		fseek(file, 0, SEEK_SET);
		fread(&elf_header64, sizeof(Elf64_Ehdr), 1, file);
	}

	/* Leer la posición de la tabla de secciones */
	section_table_offset = is_32bit ? elf_header32.e_shoff : elf_header64.e_shoff;
	/* Create sectHdr and SectNames */
	if (is_32bit)
	{
		if (elf_header32.e_ident[EI_DATA] == ELFDATA2LSB)
		{
			read_elf32_be_section(&section_header32);
		fseek(file, elf_header32.e_shoff + elf_header32.e_shstrndx * elf_header32.e_shentsize, SEEK_SET);
		SectNames = get_section_name32(section_header32, file);
		}

		if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
		{
			read_elf32_be_section(&section_header32);
		fseek(file, elf_header32.e_shoff + elf_header32.e_shstrndx * elf_header32.e_shentsize, SEEK_SET);
		SectNames = get_section_name32_big(section_header32, file);
		}
	}
	else
	{
		fseek(file, elf_header64.e_shoff + elf_header64.e_shstrndx * elf_header64.e_shentsize, SEEK_SET);
		SectNames = get_section_name64(section_header64, file);
	}
	/* Imprimir la información sobre la tabla de secciones y encabezado */
	printf("There are %u section headers, starting at offset 0x%lx:\n\n",
		is_32bit ? elf_header32.e_shnum : elf_header64.e_shnum,
		section_table_offset);
	if (is_32bit)
	{
		fseek(file, elf_header32.e_shoff, SEEK_SET);
		printf("Section Headers:\n");
		printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
	}
	else
	{
		fseek(file, elf_header64.e_shoff, SEEK_SET);
		printf("Section Headers:\n");
		printf("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");
	}

	if (is_32bit)
	{
		Elf32_Shdr section_header32;

		for (index = 0; index < elf_header32.e_shnum; index++)
		{
			char* name = "";

			fread(&section_header32, sizeof(Elf32_Shdr), 1, file);

			if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
				read_elf32_be_section(&section_header32);

			if (section_header32.sh_name)
				name = SectNames + section_header32.sh_name;
			print_Section_Info_32bits(index, section_header32, name);
		}
		printKeyToFlags_32bits();
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
			print_Section_Info_64bits(index, section_header64, name);
		}
		printKeyToFlags_64bits();
	}
	fclose(file);
	return (0);
}


void printKeyToFlags_32bits()
{
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
}

void printKeyToFlags_64bits()
{
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
}

void print_Section_Info_32bits(int index, Elf32_Shdr section_header, char *name)
{
	const char *flags = getSectionFlags(section_header.sh_flags);
	printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %s %2d %3d %2d\n",
		index, name, getSectionTypeName(section_header.sh_type),
		section_header.sh_addr, section_header.sh_offset,
		section_header.sh_size, section_header.sh_entsize, flags,
		section_header.sh_link, section_header.sh_info, section_header.sh_addralign);
}

void print_Section_Info_64bits(int index, Elf64_Shdr section_header, char *name)
{
	const char *flags = getSectionFlags(section_header.sh_flags);
	printf("  [%2d] %-17s %-15s %016lx %06lx %06lx %02lx %s %2d %3d %2ld\n",
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
	if (sh_flags & SHF_STRINGS) flags[i--] = 'S';

	if (sh_flags & SHF_INFO_LINK) flags[i--] = 'I';

	if (sh_flags & SHF_MERGE) flags[i--] = 'M';

	if (sh_flags & SHF_EXECINSTR) flags[i--] = 'X';

	if (sh_flags & SHF_ALLOC) flags[i--] = 'A';

	if (sh_flags & SHF_WRITE) flags[i--] = 'W';

	if (sh_flags & SHF_EXCLUDE) flags[i--] = 'E';

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

char *get_section_name32(Elf32_Shdr section_header, FILE *file)
{
	char* SectNames = NULL;
	fread(&section_header, 1, sizeof(section_header), file);
	SectNames = (char *)malloc(section_header.sh_size);
	fseek(file, section_header.sh_offset, SEEK_SET);
	fread(SectNames, 1, section_header.sh_size, file);
	return (SectNames);
}

char *get_section_name32_big(Elf32_Shdr section_header, FILE *file)
{
	char* SectNames = NULL;
	fread(&section_header, 1, sizeof(section_header), file);
	read_elf32_be_section(&section_header);
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

void read_elf32_be_section(Elf32_Shdr *section_header32)
{
	section_header32->sh_name = my_be32toh(section_header32->sh_name);
	section_header32->sh_type = my_be32toh(section_header32->sh_type);
	section_header32->sh_addr = my_be32toh(section_header32->sh_addr);
	section_header32->sh_offset = my_be32toh(section_header32->sh_offset);
	section_header32->sh_size = my_be32toh(section_header32->sh_size);
	section_header32->sh_entsize = my_be32toh(section_header32->sh_entsize);
	section_header32->sh_flags = my_be32toh(section_header32->sh_flags);
	section_header32->sh_link = my_be32toh(section_header32->sh_link);
	section_header32->sh_info = my_be32toh(section_header32->sh_info);
	section_header32->sh_addralign = my_be32toh(section_header32->sh_addralign);
}

void read_elf32_be_header(Elf32_Ehdr *ehdr)
{
	ehdr->e_type = my_be16toh(ehdr->e_type);
	ehdr->e_machine = my_be16toh(ehdr->e_machine);
	ehdr->e_version = my_be32toh(ehdr->e_version);
	ehdr->e_entry = my_be32toh(ehdr->e_entry);
	ehdr->e_phoff = my_be32toh(ehdr->e_phoff);
	ehdr->e_shoff = my_be32toh(ehdr->e_shoff);
	ehdr->e_flags = my_be32toh(ehdr->e_flags);
	ehdr->e_ehsize = my_be16toh(ehdr->e_ehsize);
	ehdr->e_phentsize = my_be16toh(ehdr->e_phentsize);
	ehdr->e_phnum = my_be16toh(ehdr->e_phnum);
	ehdr->e_shentsize = my_be16toh(ehdr->e_shentsize);
	ehdr->e_shnum = my_be16toh(ehdr->e_shnum);
	ehdr->e_shstrndx = my_be16toh(ehdr->e_shstrndx);
}

uint16_t my_be16toh(uint16_t value)
{
	return (((value >> 8) & 0xFF) | ((value & 0xFF) << 8));
}

uint32_t my_be32toh(uint32_t value)
{
	return (((value >> 24) & 0xFF) |
		((value >>  8) & 0xFF00) |
		((value & 0xFF00) <<  8) |
		((value & 0xFF) << 24));
}
