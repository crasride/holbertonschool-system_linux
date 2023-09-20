#include "hreadelf.h"


int main(int argc, char *argv[])
{
	FILE *file = NULL;
	ElfHeader elf_header;
	int is_32bit = 0;
	int i = 0;

	if (argc != 2)
	{
		fprintf(stderr, "Uso: %s elf_filename\n", argv[0]);
		return 1;
	}

	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		perror("No se puede abrir el archivo");
		return 1;
	}

	/* Leer el encabezado ELF principal */
	fread(&elf_header, sizeof(ElfHeader), 1, file);

	/* Verificar si es un archivo ELF de 32 o 64 bits */
	if (elf_header.ehdr.ehdr32.e_ident[EI_CLASS] == ELFCLASS32)
	{
		is_32bit = 1;
		printf("32bits\n");
	}
	else if (elf_header.ehdr.ehdr64.e_ident[EI_CLASS] == ELFCLASS64)
	{
		is_32bit = 0;
		printf("64bits\n");

	}
	else
	{
		fprintf(stderr, "El archivo ELF tiene una clase desconocida.\n");
		fclose(file);
		return 1;
	}
	/* Imprimir información del encabezado ELF */
	print_elf_info(&elf_header, is_32bit);

	/* Leer y mostrar la información de los encabezados del programa ELF */
	for (i = 0; i < (is_32bit ? elf_header.ehdr.ehdr32.e_phnum : elf_header.ehdr.ehdr64.e_phnum); i++)
	{

		if (is_32bit)
		{
			Elf32_Phdr program_header;
			fread(&program_header, sizeof(Elf32_Phdr), 1, file);
			print_program_header_info_32(&program_header);
		}
		else
		{
			Elf64_Phdr program_header;
			fread(&program_header, sizeof(Elf64_Phdr), 1, file);
			print_program_header_info_64(&program_header);
		}
	}
	fclose(file);
	return 0;
}

void print_elf_info(ElfHeader *elf_header, int is_32bit)
{
	printf("Elf file type is %s\n", getElfTypeName(is_32bit ? elf_header->ehdr.ehdr32.e_type : elf_header->ehdr.ehdr64.e_type));

	if (is_32bit)
	{
		printf("Entry point 0x%x\n", elf_header->ehdr.ehdr32.e_entry);
		printf("File is %s endian\n", (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2LSB) ? "little" : "big");
	}
	else
	{
		printf("Entry point 0x%1lx\n", elf_header->ehdr.ehdr64.e_entry);
		printf("File is %s endian\n", (elf_header->ehdr.ehdr64.e_ident[EI_DATA] == ELFDATA2LSB) ? "little" : "big");
	}

	printf("There are %d program headers, starting at offset %ld\n\n",
		is_32bit ? elf_header->ehdr.ehdr32.e_phnum : elf_header->ehdr.ehdr64.e_phnum,
		is_32bit ? (long)elf_header->ehdr.ehdr32.e_phoff : (long)elf_header->ehdr.ehdr64.e_phoff);

	printf("Program Headers:\n");
	printf("  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align\n");
}

const char *getElfTypeName(uint16_t e_type)
{
	switch (e_type)
	{
	case ET_NONE: return "NONE";
	case ET_REL: return "REL";
	case ET_EXEC: return "EXEC (Executable file)";
	case ET_DYN: return "DYN";
	case ET_CORE: return "CORE";
	default: return "UNKNOWN";
	}
}

const char *getProgramHeaderTypeName32(uint32_t p_type)
{
	switch (p_type)
	{
	case PT_NULL: return "NULL";
	case PT_LOAD: return "LOAD";
	case PT_DYNAMIC: return "DYNAMIC";
	case PT_INTERP: return "INTERP";
	case PT_NOTE: return "NOTE";
	case PT_SHLIB: return "SHLIB";
	case PT_PHDR: return "PHDR";
	case PT_TLS: return "TLS";
	default: return "UNKNOWN";
	}
}
const char *getProgramHeaderTypeName64(uint64_t p_type)
{
	switch (p_type)
	{
	case PT_NULL: return "NULL";
	case PT_LOAD: return "LOAD";
	case PT_DYNAMIC: return "DYNAMIC";
	case PT_INTERP: return "INTERP";
	case PT_NOTE: return "NOTE";
	case PT_SHLIB: return "SHLIB";
	case PT_PHDR: return "PHDR";
	case PT_TLS: return "TLS";
	case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
	case PT_GNU_STACK: return "GNU_STACK";
	case PT_GNU_RELRO: return "GNU_RELRO";
	default: return "UNKNOWN";
	}
}

void print_program_header_info_32(Elf32_Phdr *program_header)
{
	printf("  %-14s 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x %c%c%c 0x%08x\n",
		getProgramHeaderTypeName32(program_header->p_type),
		(unsigned int)program_header->p_offset,
		(unsigned int)program_header->p_vaddr,
		(unsigned int)program_header->p_paddr,
		(unsigned int)program_header->p_filesz,
		(unsigned int)program_header->p_memsz,
		(program_header->p_flags & PF_R) ? 'R' : ' ',
		(program_header->p_flags & PF_W) ? 'W' : ' ',
		(program_header->p_flags & PF_X) ? 'E' : ' ',
		program_header->p_align);
}


void print_program_header_info_64(Elf64_Phdr *program_header)
{
	printf("  %-14s 0x%06lx 0x%016lx 0x%016lx 0x%06lx 0x%06lx %c%c%c 0x%01lx\n",
		getProgramHeaderTypeName64(program_header->p_type),
		(unsigned long)program_header->p_offset,
		(unsigned long)program_header->p_vaddr,
		(unsigned long)program_header->p_paddr,
		(unsigned long)program_header->p_filesz,
		(unsigned long)program_header->p_memsz,
		(program_header->p_flags & PF_R) ? 'R' : ' ',
		(program_header->p_flags & PF_W) ? 'W' : ' ',
		(program_header->p_flags & PF_X) ? 'E' : ' ',
		(unsigned long)program_header->p_align);
}
