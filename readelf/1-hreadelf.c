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
	char *SectNames = NULL;
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
			read_elf32_be_header(&elf_header32);
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
			fseek(file, elf_header32.e_shoff + elf_header32.e_shstrndx *
					elf_header32.e_shentsize, SEEK_SET);
			SectNames = get_section_name32(section_header32, file);
		}
		if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
		{
			read_elf32_be_section(&section_header32);
			fseek(file, elf_header32.e_shoff + elf_header32.e_shstrndx *
					elf_header32.e_shentsize, SEEK_SET);
			SectNames = get_section_name32_big(section_header32, file);
		}
	}
	else
	{
		fseek(file, elf_header64.e_shoff + elf_header64.e_shstrndx *
				elf_header64.e_shentsize, SEEK_SET);
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
		printf("  [Nr] Name              Type            Addr     Off    Size");
		printf("   ES Flg Lk Inf Al\n");
	}
	else
	{
		fseek(file, elf_header64.e_shoff, SEEK_SET);
		printf("Section Headers:\n");
		printf("  [Nr] Name              Type            Address          Off");
		printf("    Size   ES Flg Lk Inf Al\n");
	}

	if (is_32bit)
	{
		Elf32_Shdr section_header32;

		for (index = 0; index < elf_header32.e_shnum; index++)
		{
			char *name = "";

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
			char *name = "";

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
