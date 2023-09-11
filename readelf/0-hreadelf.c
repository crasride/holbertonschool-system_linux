#include <elf.h>
#include "hreadelf.h"
#include <stdio.h>
#include <stdlib.h>


const char *get_osabi_name(uint8_t osabi)
{
	switch (osabi)
	{
		case ELFOSABI_NONE:
			return ("UNIX - System V");
		case ELFOSABI_NETBSD:
			return ("UNIX - NetBSD");
		case ELFOSABI_SOLARIS:
			return ("UNIX - Solaris");
		default:
			return ("<unknown: 53>");
	}
}

const char *get_osabi_version(uint8_t version)
{
	switch (version)
	{
		case ELFOSABI_NONE:
			return "0";
		case ELFOSABI_NETBSD:
			return "0";
		case ELFOSABI_SOLARIS:
			return "1";
		default:
			return "unknown version";
	}
}
const char *get_osabi_type(uint16_t type)
{
	switch (type)
	{
		case ET_NONE:
			return ("An unknown type");
		case ET_DYN:
			return ("DYN (Shared object file)");
		case ET_EXEC:
			return ("EXEC (Executable file)");
		case ET_REL:
			return ("REL (Relocatable file)");
		case ET_CORE:
			return ("A core file");
		default:
			return ("unknown type");
	}
}

const char *get_osabi_machine(uint16_t machine)
{
	switch (machine)
	{
		case EM_SPARC:
			return ("Sparc");
		case EM_SPARCV9:
			return ("Sparc");
		case EM_SPARC32PLUS:
			return ("Sparc");
		case EM_X86_64:
			return ("Advanced Micro Devices X86-64");
		case EM_386:
			return ("Intel 80386");
		case EM_NONE:
			return ("An unknown machine");
		default:
			return ("unknown machine");
	}
}

const char *get_osabi_data(uint8_t data)
{
	switch (data)
	{
		case ELFDATA2LSB:
			return ("2's complement, little endian");
		case ELFDATA2MSB:
			return ("2's complement, big endian");
		case ELFDATANONE:
			return ("Unknown data format");
		default:
			return ("unknown data");
	}
}


void print_elf32_header(Elf32_Header *elf32)
{
	int i;

	printf("ELF Header:\n");
	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x ", elf32->ehdr.e_ident[i]);
	}
	printf("\n");
	printf("  Class:                             ELF32\n");
	printf("  Data:                              %s\n", get_osabi_data(elf32->ehdr.e_ident[EI_DATA]));
	printf("  Version:                           %d (current)\n", elf32->ehdr.e_version);
	printf("  OS/ABI:                            %s\n", get_osabi_name(elf32->ehdr.e_ident[EI_OSABI]));
	printf("  ABI Version:                       %s\n", get_osabi_version(elf32->ehdr.e_ident[EI_OSABI]));
	printf("  Type:                              %s\n", get_osabi_type(elf32->ehdr.e_type));
	printf("  Machine:                           %s\n", get_osabi_machine(elf32->ehdr.e_machine));
	printf("  Version:                           0x%x\n", elf32->ehdr.e_version);
	printf("  Entry point address:               0x%x\n", elf32->ehdr.e_entry);
	printf("  Start of program headers:          %d (bytes into file)\n", elf32->ehdr.e_phoff);
	printf("  Start of section headers:          %d (bytes into file)\n", elf32->ehdr.e_shoff);
	printf("  Flags:                             0x%x\n", elf32->ehdr.e_flags);
	printf("  Size of this header:               %d (bytes)\n", elf32->ehdr.e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n", elf32->ehdr.e_phentsize);
	printf("  Number of program headers:         %d\n", elf32->ehdr.e_phnum);
	printf("  Size of section headers:           %d (bytes)\n", elf32->ehdr.e_shentsize);
	printf("  Number of section headers:         %d\n", elf32->ehdr.e_shnum);
	printf("  Section header string table index: %d\n", elf32->ehdr.e_shstrndx);
}

void print_elf64_header(Elf64_Header *elf64)
{
	int i;
	printf("ELF Header:\n");
	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x ", elf64->ehdr.e_ident[i]);
	}
	printf("\n");
	printf("  Class:                             ELF64\n");
	printf("  Data:                              %s\n", get_osabi_data(elf64->ehdr.e_ident[EI_DATA]));
	printf("  Version:                           %d (current)\n", elf64->ehdr.e_version);
	printf("  OS/ABI:                            %s\n", get_osabi_name(elf64->ehdr.e_ident[EI_OSABI]));
	/* printf("  ABI Version:                       %d\n", elf64->ehdr.e_ident[EI_OSABI]); */
	printf("  ABI Version:                       %s\n", get_osabi_version(elf64->ehdr.e_ident[EI_OSABI]));
	printf("  Type:                              %s\n", get_osabi_type(elf64->ehdr.e_type));
	printf("  Machine:                           %s\n", get_osabi_machine(elf64->ehdr.e_machine));
	printf("  Version:                           0x%x\n", elf64->ehdr.e_version);
	printf("  Entry point address:               0x%lx\n", elf64->ehdr.e_entry);
	printf("  Start of program headers:          %ld (bytes into file)\n", (long)elf64->ehdr.e_phoff);
	printf("  Start of section headers:          %ld (bytes into file)\n", elf64->ehdr.e_shoff);
	printf("  Flags:                             0x%x\n", elf64->ehdr.e_flags);
	printf("  Size of this header:               %d (bytes)\n", elf64->ehdr.e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n", elf64->ehdr.e_phentsize);
	printf("  Number of program headers:         %d\n", elf64->ehdr.e_phnum);
	printf("  Size of section headers:           %d (bytes)\n", elf64->ehdr.e_shentsize);
	printf("  Number of section headers:         %d\n", elf64->ehdr.e_shnum);
	printf("  Section header string table index: %d\n", elf64->ehdr.e_shstrndx);
}


int main(int argc, char *argv[])
{
	FILE *file = NULL;
	Elf32_Header elf32;
	Elf64_Header elf64;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
		return (1);
	}

	file = fopen(argv[1], "rb");

	if (!file)
	{
		perror("Error opening the file");
		return (1);
	}

	/* Read the 32-bit ELF header */
	fread(&elf32, sizeof(Elf32_Header), 1, file);


	if (elf32.ehdr.e_ident[EI_CLASS] == ELFCLASS64)
	{
		/* This is a 64-bit ELF file, so read the 64-bit ELF header */
		fseek(file, 0, SEEK_SET); /* Move the file pointer back to the beginning */
		fread(&elf64, sizeof(Elf64_Header), 1, file);
		print_elf64_header(&elf64);
	}
	else
	{

		print_elf32_header(&elf32);
	}

	fclose(file);

	return (0);
}
