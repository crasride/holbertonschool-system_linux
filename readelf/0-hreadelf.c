#include <elf.h>
#include "hreadelf.h"
#include <endian.h>


/**
 * print_elf32_header - Print the details of an ELF32 header.
 * @elf32: Pointer to an ELF32 header structure.
 *
 * This function takes a pointer to an ELF32 header structure and prints its
 * details to the console.
 * It provides information such as the ELF magic, class (ELF32), data format,
 * OS/ABI, ABI version, file type, machine architecture, entry point address,
 * program headers, section headers, flags, and more.
 */
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
	printf("  Data:                              %s\n",
			get_osabi_data(elf32->ehdr.e_ident[EI_DATA]));
	printf("  Version:                           %d (current)\n",
			elf32->ehdr.e_version);
	printf("  OS/ABI:                            %s\n",
			get_osabi_name(elf32->ehdr.e_ident[EI_OSABI]));
	printf("  ABI Version:                       %s\n",
			get_osabi_version(elf32->ehdr.e_ident[EI_OSABI]));
	printf("  Type:                              %s\n",
			get_osabi_type(elf32->ehdr.e_type));
	printf("  Machine:                           %s\n",
			get_osabi_machine(elf32->ehdr.e_machine));
	printf("  Version:                           0x%x\n", elf32->ehdr.e_version);
	printf("  Entry point address:               0x%x\n", elf32->ehdr.e_entry);
	printf("  Start of program headers:          %d (bytes into file)\n",
			elf32->ehdr.e_phoff);
	printf("  Start of section headers:          %d (bytes into file)\n",
			elf32->ehdr.e_shoff);
	printf("  Flags:                             0x%x\n", elf32->ehdr.e_flags);
	printf("  Size of this header:               %d (bytes)\n",
			elf32->ehdr.e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n",
			elf32->ehdr.e_phentsize);
	printf("  Number of program headers:         %d\n", elf32->ehdr.e_phnum);
	printf("  Size of section headers:           %d (bytes)\n",
			elf32->ehdr.e_shentsize);
	printf("  Number of section headers:         %d\n", elf32->ehdr.e_shnum);
	printf("  Section header string table index: %d\n", elf32->ehdr.e_shstrndx);
}

/**
 * print_elf64_header - Print the details of an ELF64 header.
 * @elf64: Pointer to an ELF64 header structure.
 *
 * This function takes a pointer to an ELF64 header structure and prints its
 * details to the console.
 * It provides information such as the ELF magic, class (ELF64), data format,
 * OS/ABI, ABI version, file type, machine architecture, entry point address,
 * program headers, section headers, flags, and more.
 */

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
	printf("  Data:                              %s\n",
			get_osabi_data(elf64->ehdr.e_ident[EI_DATA]));
	printf("  Version:                           %d (current)\n",
			elf64->ehdr.e_version);
	printf("  OS/ABI:                            %s\n",
			get_osabi_name(elf64->ehdr.e_ident[EI_OSABI]));
	/* printf("  ABI Version:        %d\n", elf64->ehdr.e_ident[EI_OSABI]); */
	printf("  ABI Version:                       %s\n",
			get_osabi_version(elf64->ehdr.e_ident[EI_OSABI]));
	printf("  Type:                              %s\n",
			get_osabi_type(elf64->ehdr.e_type));
	printf("  Machine:                           %s\n",
			get_osabi_machine(elf64->ehdr.e_machine));
	printf("  Version:                           0x%x\n", elf64->ehdr.e_version);
	printf("  Entry point address:               0x%lx\n", elf64->ehdr.e_entry);
	printf("  Start of program headers:          %ld (bytes into file)\n",
			(long)elf64->ehdr.e_phoff);
	printf("  Start of section headers:          %ld (bytes into file)\n",
			elf64->ehdr.e_shoff);
	printf("  Flags:                             0x%x\n", elf64->ehdr.e_flags);
	printf("  Size of this header:               %d (bytes)\n",
			elf64->ehdr.e_ehsize);
	printf("  Size of program headers:           %d (bytes)\n",
			elf64->ehdr.e_phentsize);
	printf("  Number of program headers:         %d\n",
			elf64->ehdr.e_phnum);
	printf("  Size of section headers:           %d (bytes)\n",
			elf64->ehdr.e_shentsize);
	printf("  Number of section headers:         %d\n", elf64->ehdr.e_shnum);
	printf("  Section header string table index: %d\n", elf64->ehdr.e_shstrndx);
}


void read_elf32_be_header0(Elf32_Header *elf32, FILE *file)
{
	/* Leer el encabezado ELF de 32 bits en formato big-endian */
	fread(elf32, sizeof(Elf32_Header), 1, file);

	/* Ajustar valores si es necesario debido a la conversión de big-endian */
	elf32->ehdr.e_type = my_be16toh(elf32->ehdr.e_type);
	elf32->ehdr.e_machine = my_be16toh(elf32->ehdr.e_machine);
	elf32->ehdr.e_version = my_be32toh(elf32->ehdr.e_version);
	elf32->ehdr.e_entry = my_be32toh(elf32->ehdr.e_entry);
	elf32->ehdr.e_phoff = my_be32toh(elf32->ehdr.e_phoff);
	elf32->ehdr.e_shoff = my_be32toh(elf32->ehdr.e_shoff);
	elf32->ehdr.e_flags = my_be32toh(elf32->ehdr.e_flags);
	elf32->ehdr.e_ehsize = my_be16toh(elf32->ehdr.e_ehsize);
	elf32->ehdr.e_phentsize = my_be16toh(elf32->ehdr.e_phentsize);
	elf32->ehdr.e_phnum = my_be16toh(elf32->ehdr.e_phnum);
	elf32->ehdr.e_shentsize = my_be16toh(elf32->ehdr.e_shentsize);
	elf32->ehdr.e_shnum = my_be16toh(elf32->ehdr.e_shnum);
	elf32->ehdr.e_shstrndx = my_be16toh(elf32->ehdr.e_shstrndx);
}

int main(int argc, char *argv[])
{
	FILE *file = NULL;
	Elf32_Header elf32;
	Elf64_Header elf64;
	uint8_t data;

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
	fseek(file, EI_DATA, SEEK_SET);/*valor EI_DATA little-endian o big-endian */
	fread(&data, sizeof(uint8_t), 1, file);
	fseek(file, 0, SEEK_SET); /* Mover puntero del archivo vuelta al principio */
	if (data == ELFDATA2MSB)
	{
		read_elf32_be_header0(&elf32, file);/* ELF formato big-endian 32 bits */
		print_elf32_header(&elf32);
	}
	else
	{
		fread(&elf32, sizeof(Elf32_Header), 1, file);/*No big de 32 o 64 bits*/
		if (elf32.ehdr.e_ident[EI_CLASS] == ELFCLASS64)
		{
			fseek(file, 0, SEEK_SET);/* Archivo ELF de 64 bits */
			fread(&elf64, sizeof(Elf64_Header), 1, file);
			print_elf64_header(&elf64);
		}
		else if (elf32.ehdr.e_ident[EI_CLASS] == ELFCLASS32)
			print_elf32_header(&elf32);/*ELF de 32 bitsformato little-endian */
		else
			fprintf(stderr, "Format not supported\n");
		}
	fclose(file);
	return (0);
}
