#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>
#include "hobjdump.h"


void print_flag(int *flag_printed, unsigned long flags, unsigned long flag,
				const char *flag_name)
{
	if (flags & flag)
	{
		if (*flag_printed)
			printf(", ");
		printf("%s", flag_name);
		*flag_printed = 1;
	}
}


uint32_t my_be32toh(uint32_t value, int is_big_endian)
{
	if (is_big_endian)
		return (((value >> 24) & 0xFF) |
				((value >> 8) & 0xFF00) |
				((value & 0xFF00) << 8) |
				((value & 0xFF) << 24));
	else
		return (value);

}

uint16_t my_be16toh(uint16_t value, int is_big_endian)
{
	if (is_big_endian)
		return (((value >> 8) & 0xFF) | ((value & 0xFF) << 8));
	else
		return (value);
}


void print_elf_header_32(Elf32_Ehdr *ehdr, const char *filename)
{
	const char *formatted_filename = filename;
	int flag_printed = 0;
	unsigned long flags_interp = 0;
	int is_big_endian = 0;

	if (formatted_filename[0] == '.' && formatted_filename[1] == '/')
		formatted_filename += 2;

	is_big_endian = (ehdr->e_ident[EI_DATA] == ELFDATA2MSB);
	printf("%s:     file format %s\n", formatted_filename, (is_big_endian) ? "elf32-big" : "elf32-i386");
	printf("architecture: %s,", (is_big_endian) ? "UNKNOWN!" : "i386");

	if (my_be16toh(ehdr->e_type, is_big_endian) == ET_EXEC)
		flags_interp |= EXEC_P;
	if (my_be16toh(ehdr->e_type, is_big_endian) == ET_REL)
		flags_interp |= HAS_RELOC;
	if (my_be16toh(ehdr->e_type, is_big_endian) == ET_DYN)
		flags_interp |= DYNAMIC;

	flags_interp |= (ehdr->e_shnum > 0) ? HAS_SYMS : 0;
	flags_interp |= (ehdr->e_phnum > 0) ? D_PAGED : 0;

	printf(" flags 0x%08lx:\n", flags_interp);
	print_flag(&flag_printed, flags_interp, EXEC_P, "EXEC_P");
	print_flag(&flag_printed, flags_interp, HAS_RELOC, "HAS_RELOC");
	print_flag(&flag_printed, flags_interp, HAS_SYMS, "HAS_SYMS");
	print_flag(&flag_printed, flags_interp, DYNAMIC, "DYNAMIC");
	print_flag(&flag_printed, flags_interp, D_PAGED, "D_PAGED");
	printf("\n");
	printf("start address 0x%08lx\n", (unsigned long)(my_be32toh(ehdr->e_entry, is_big_endian)));
}


void print_elf_header_64(Elf64_Ehdr *ehdr, const char *filename)
{
	const char *formatted_filename = filename;
	int flag_printed = 0;
	unsigned long flags_interp = 0;

	if (formatted_filename[0] == '.' && formatted_filename[1] == '/')
		formatted_filename += 2;

	printf("%s:     file format elf64-x86-64\n", formatted_filename);
	printf("architecture: i386:x86-64");

	if (ehdr->e_type == ET_EXEC)
		flags_interp |= EXEC_P;
	if (ehdr->e_type == ET_REL)
		flags_interp |= HAS_RELOC;
	if (ehdr->e_type == ET_DYN)
		flags_interp |= DYNAMIC;

	flags_interp |= (ehdr->e_shnum > 0) ? HAS_SYMS : 0;
	flags_interp |= (ehdr->e_phnum > 0) ? D_PAGED : 0;

	printf(" flags 0x%08lx:\n", flags_interp);

	print_flag(&flag_printed, flags_interp, EXEC_P, "EXEC_P");
	print_flag(&flag_printed, flags_interp, HAS_RELOC, "HAS_RELOC");
	print_flag(&flag_printed, flags_interp, HAS_SYMS, "HAS_SYMS");
	print_flag(&flag_printed, flags_interp, DYNAMIC, "DYNAMIC");
	print_flag(&flag_printed, flags_interp, D_PAGED, "D_PAGED");

	printf("\n");

	printf("start address 0x%016lx\n", (unsigned long)ehdr->e_entry);
}

int analyze_64bit_elf(Elf64_Ehdr *ehdr, const char *filename)
{
	if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
	{
		print_elf_header_64(ehdr, filename);
	}
	else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		printf("big-endian64.\n");
	}
	else
	{
		printf("endianness unknown.\n");
	}

	return (0);
}

int analyze_file(const char *filename)
{
	int fd;
	void *map;
	size_t file_size;
	Elf32_Ehdr *ehdr32;
	Elf64_Ehdr *ehdr64;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}

	file_size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	map = mmap(0, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED)
	{
		perror("mmap");
		return (1);
	}
	ehdr32 = (Elf32_Ehdr *)map;
	ehdr64 = (Elf64_Ehdr *)map;

	if (ehdr32->e_ident[EI_CLASS] == ELFCLASS32)
	{
		analyze_32bit_elf(ehdr32, filename);
	}
	else if (ehdr64->e_ident[EI_CLASS] == ELFCLASS64)
		analyze_64bit_elf(ehdr64, filename);
	else
	{
		fprintf(stderr, "No es un archivo ELF válido.\n");
		return (1);
	}

	close(fd);
	munmap(map, file_size);
	return (0);
}

int main(int argc, char *argv[])
{
	int i;

	if (argc < 2)
	{
		fprintf(stderr, "hobjdump: %s [objfile...]\n", argv[0]);
		return (1);
	}

	for (i = 1; i < argc; i++)
	{

		if (analyze_file(argv[i]) != 0)
		{
			return (1);
		}
	}

	return (0);
}

int analyze_32bit_elf(Elf32_Ehdr *ehdr, const char *filename)
{
	if (ehdr->e_ident[EI_DATA] == ELFDATA2LSB)
	{
		print_elf_header_32(ehdr, filename);
	}
	else if (ehdr->e_ident[EI_DATA] == ELFDATA2MSB)
	{
		print_elf_header_32(ehdr, filename);
	}
	else
	{
		printf("endianness unknown.\n");
	}
	return (0);
}


	/* printf("e_ident[EI_MAG0]: 0x%02x\n", ehdr->e_ident[EI_MAG0]);
	printf("e_ident[EI_MAG1]: 0x%02x\n", ehdr->e_ident[EI_MAG1]);
	printf("e_ident[EI_MAG2]: 0x%02x\n", ehdr->e_ident[EI_MAG2]);
	printf("e_ident[EI_MAG3]: 0x%02x\n", ehdr->e_ident[EI_MAG3]);
	printf("e_ident[EI_CLASS]: 0x%02x\n", ehdr->e_ident[EI_CLASS]);
	printf("e_ident[EI_DATA]: 0x%02x\n", ehdr->e_ident[EI_DATA]);
	printf("e_ident[EI_VERSION]: 0x%02x\n", ehdr->e_ident[EI_VERSION]);
	printf("e_type: 0x%04x\n", ehdr->e_type);
	printf("e_machine: 0x%04x\n", ehdr->e_machine);
	printf("e_version: 0x%08x\n", ehdr->e_version);
	printf("e_entry: 0x%016lx\n", (unsigned long)ehdr->e_entry);
	printf("e_phoff: 0x%016lx\n", (unsigned long)ehdr->e_phoff);
	printf("e_shoff: 0x%016lx\n", (unsigned long)ehdr->e_shoff);
	printf("e_flags: 0x%08x\n", ehdr->e_flags);
	printf("e_ehsize: 0x%04x\n", ehdr->e_ehsize);
	printf("e_phentsize: 0x%04x\n", ehdr->e_phentsize);
	printf("e_phnum: 0x%04x\n", ehdr->e_phnum);
	printf("e_shentsize: 0x%04x\n", ehdr->e_shentsize);
	printf("e_shnum: 0x%04x\n", ehdr->e_shnum);
	printf("e_shstrndx: 0x%04x\n", ehdr->e_shstrndx); */
