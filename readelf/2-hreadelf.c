#include "hreadelf.h"


int main(int argc, char *argv[])
{
	FILE *file = NULL;
	ElfHeader elf_header;
	int is_32bit = 0;
	int i = 0;
	size_t program_header_size;
	char interp[MAX_INTERP_SIZE];

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
		if (elf_header.ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2LSB)
		{
		}
		else if (elf_header.ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
		{
			read_elf32_be_header(&elf_header.ehdr.ehdr32);
		}
	}
	else if (elf_header.ehdr.ehdr64.e_ident[EI_CLASS] == ELFCLASS64)
	{
		is_32bit = 0;
	}
	else
	{
		fprintf(stderr, "El archivo ELF tiene una clase desconocida.\n");
		fclose(file);
		return 1;
	}
	/* Imprimir información del encabezado ELF */
	print_elf_info(&elf_header, is_32bit);

	 /* Calcular el tamaño de un encabezado de programa */
	program_header_size = is_32bit ? sizeof(Elf32_Phdr) : sizeof(Elf64_Phdr);

	/* Mover el puntero de archivo al inicio de los encabezados de programa */
	fseek(file, (is_32bit ? elf_header.ehdr.ehdr32.e_phoff : elf_header.ehdr.ehdr64.e_phoff), SEEK_SET);

	/* Leer y mostrar la información de los encabezados del programa ELF */
	for (i = 0; i < (is_32bit ? elf_header.ehdr.ehdr32.e_phnum : elf_header.ehdr.ehdr64.e_phnum); i++)
	{
		if (is_32bit)
		{
			Elf32_Phdr program_header;
			fread(&program_header, program_header_size, 1, file);
			if (elf_header.ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
				read_elf32_be_prog(&program_header);
			print_program_header_info_32(&program_header);

			if (program_header.p_type == PT_INTERP)
			{
				if (program_header.p_filesz <= MAX_INTERP_SIZE)
				{
					/* Guardo la posicion actual */
					long current_pos = ftell(file);

					/* Mover el puntero posicio del programa intérprete */
					fseek(file, program_header.p_offset, SEEK_SET);

					/* Leer el programa intérprete en la matriz interp */
					fread(interp, program_header.p_filesz, 1, file);
					interp[program_header.p_filesz] = '\0';

					print_interpreter_info(interp);

					/* Recupero la posicion guardada */
					fseek(file, current_pos, SEEK_SET);
				}
				else
				{
					fprintf(stderr, "MAX_INTERP_SIZE.\n");
					fclose(file);
					return 1;
				}
			}
		}
		else
		{
			Elf64_Phdr program_header;
			fread(&program_header, program_header_size, 1, file);
			print_program_header_info_64(&program_header);

			if (program_header.p_type == PT_INTERP)
			{
				if (program_header.p_filesz <= MAX_INTERP_SIZE)
				{
					/* Guardo la posicion actual */
					long current_pos = ftell(file);

					fseek(file, program_header.p_offset, SEEK_SET);
					fread(interp, program_header.p_filesz, 1, file);
					interp[program_header.p_filesz] = '\0';
					print_interpreter_info(interp);

					/* Recupero la posicion guardada */
					fseek(file, current_pos, SEEK_SET);
				}
				else
				{
					fprintf(stderr, "MAX_INTERP_SIZE.\n");
					fclose(file);
					return 1;
				}
			}
		}
	}

	/* Section to Segment mapping cucu*/
	if (elf_header.ehdr.ehdr32.e_ident[EI_CLASS] == ELFCLASS32)
	{
		is_32bit = 1;
		if (elf_header.ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2LSB)
		{
			createSectionToSegmentMapping32(file, &elf_header, is_32bit);
		}
		else if (elf_header.ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
		{

			createSectionToSegmentMapping32(file, &elf_header, is_32bit);
		}
	}
	else if (elf_header.ehdr.ehdr64.e_ident[EI_CLASS] == ELFCLASS64)
	{
		is_32bit = 0;
		createSectionToSegmentMapping64(file, &elf_header, is_32bit);
	}




	fclose(file);
	return 0;
}

void createSectionToSegmentMapping64(FILE *file, ElfHeader *elf_header, int is_32bit)
{
	int i, j;
	Elf64_Shdr shstrtab_header;
	Elf64_Shdr section_headers[MAX_INTERP_SIZE];
	char *shstrtab = NULL;
	char sections[MAX_INTERP_SIZE] = "";

	SectionToSegmentMapping *mapping = malloc(elf_header->ehdr.ehdr64.e_phnum * sizeof(SectionToSegmentMapping));

	for (i = 0; i < elf_header->ehdr.ehdr64.e_phnum; i++)
	{
		mapping[i].segment_number = i;
		strcpy(mapping[i].sections, "");
	}

	/* Obtener la tabla de cadenas de secciones */
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->ehdr.ehdr64.e_shoff) + elf_header->ehdr.ehdr64.e_shstrndx * sizeof(Elf64_Shdr), SEEK_SET);
	fread(&shstrtab_header, sizeof(Elf64_Shdr), 1, file);

	shstrtab = malloc(shstrtab_header.sh_size);
	fseek(file, shstrtab_header.sh_offset, SEEK_SET);
	fread(shstrtab, shstrtab_header.sh_size, 1, file);
	/* Leer section headers */
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->ehdr.ehdr64.e_shoff), SEEK_SET);
	fread(section_headers, sizeof(Elf64_Shdr), elf_header->ehdr.ehdr64.e_shnum, file);

	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_phoff : elf_header->ehdr.ehdr64.e_phoff), SEEK_SET);
	for (i = 0; i < (is_32bit ? elf_header->ehdr.ehdr32.e_phnum : elf_header->ehdr.ehdr64.e_phnum); i++)
	{
		Elf64_Phdr program_header;
		fread(&program_header, sizeof(Elf64_Phdr), 1, file);
		strcpy(sections, "");

		for (j = 0; j < elf_header->ehdr.ehdr64.e_shnum; j++)
		{
			Elf64_Shdr section_header = section_headers[j];
			/* Obtener el nombre de la sección utilizando la tabla de cadenas de secciones */
			const char *section_name = shstrtab + section_header.sh_name;

			if (section_header.sh_addr >= program_header.p_vaddr &&
				section_header.sh_addr + section_header.sh_size <= program_header.p_vaddr + program_header.p_memsz)
			{
				if (strlen(sections) > 0)
				{
					strcat(sections, " ");
				}
				strcat(sections, section_name);
			}
		}
		strcpy(mapping[i].sections, sections);
	}
	free(shstrtab);
	/* Imprimir la tabla de mapeo de secciones a segmentos */
	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	for (i = 0; i < elf_header->ehdr.ehdr64.e_phnum; i++)
	{
		printf("   %02d     %s\n", mapping[i].segment_number, mapping[i].sections);
	}
	free(mapping);
}

void createSectionToSegmentMapping32(FILE *file, ElfHeader *elf_header, int is_32bit)
{
	int i, j;
	Elf32_Shdr shstrtab_header;
	Elf32_Shdr section_headers[MAX_INTERP_SIZE];
	char *shstrtab = NULL;
	char sections[MAX_INTERP_SIZE] = "";

	SectionToSegmentMapping *mapping = malloc(elf_header->ehdr.ehdr32.e_phnum * sizeof(SectionToSegmentMapping));

	for (i = 0; i < elf_header->ehdr.ehdr32.e_phnum; i++)
	{
		mapping[i].segment_number = i;
		strcpy(mapping[i].sections, "");
	}

	/* Obtener la tabla de cadenas de secciones */
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->ehdr.ehdr64.e_shoff) + elf_header->ehdr.ehdr32.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);
	fread(&shstrtab_header, sizeof(Elf32_Shdr), 1, file);

	shstrtab = malloc(shstrtab_header.sh_size);
	fseek(file, shstrtab_header.sh_offset, SEEK_SET);
	fread(shstrtab, shstrtab_header.sh_size, 1, file);
	/* Leer section headers */
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->ehdr.ehdr64.e_shoff), SEEK_SET);
	fread(section_headers, sizeof(Elf32_Shdr), elf_header->ehdr.ehdr32.e_shnum, file);

	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_phoff : elf_header->ehdr.ehdr64.e_phoff), SEEK_SET);
	for (i = 0; i < (is_32bit ? elf_header->ehdr.ehdr32.e_phnum : elf_header->ehdr.ehdr64.e_phnum); i++)
	{
		Elf32_Phdr program_header;
		fread(&program_header, sizeof(Elf32_Phdr), 1, file);
		strcpy(sections, "");

		for (j = 0; j < elf_header->ehdr.ehdr32.e_shnum; j++)
		{
			Elf32_Shdr section_header = section_headers[j];
			/* Obtener el nombre de la sección utilizando la tabla de cadenas de secciones */
			const char *section_name = shstrtab + section_header.sh_name;

			if (section_header.sh_addr >= program_header.p_vaddr &&
				section_header.sh_addr + section_header.sh_size <= program_header.p_vaddr + program_header.p_memsz)
			{
				if (strlen(sections) > 0)
				{
					strcat(sections, " ");
				}
				strcat(sections, section_name);
			}
		}
		strcpy(mapping[i].sections, sections);
	}
	free(shstrtab);
	/* Imprimir la tabla de mapeo de secciones a segmentos */
	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	for (i = 0; i < elf_header->ehdr.ehdr32.e_phnum; i++)
	{
		printf("   %02d     %s\n", mapping[i].segment_number, mapping[i].sections);
	}
	free(mapping);
}






void print_interpreter_info(const char *interp)
{
    printf("      [Requesting program interpreter: %s]\n", interp);
}

void print_elf_info(ElfHeader *elf_header, int is_32bit)
{
	printf("Elf file type is %s\n", getElfTypeName(is_32bit ? elf_header->ehdr.ehdr32.e_type : elf_header->ehdr.ehdr64.e_type));

	if (is_32bit)
	{
		printf("Entry point 0x%x\n", elf_header->ehdr.ehdr32.e_entry);
		printf("There are %d program headers, starting at offset %ld\n\n",
		is_32bit ? elf_header->ehdr.ehdr32.e_phnum : elf_header->ehdr.ehdr64.e_phnum,
		is_32bit ? (long)elf_header->ehdr.ehdr32.e_phoff : (long)elf_header->ehdr.ehdr64.e_phoff);

	printf("Program Headers:\n");
	printf("  Type           Offset   VirtAddr   PhysAddr  FileSiz  MemSiz  Flg Align\n");

	}
	else
	{
		printf("Entry point 0x%1lx\n", elf_header->ehdr.ehdr64.e_entry);
		printf("There are %d program headers, starting at offset %ld\n\n",
		is_32bit ? elf_header->ehdr.ehdr32.e_phnum : elf_header->ehdr.ehdr64.e_phnum,
		is_32bit ? (long)elf_header->ehdr.ehdr32.e_phoff : (long)elf_header->ehdr.ehdr64.e_phoff);

	printf("Program Headers:\n");
	printf("  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align\n");
	}
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
	printf("  %-14s 0x%06x 0x%08x 0x%08x 0x%05x 0x%05x %c%c%c %#x\n",
		getProgramHeaderTypeName32(program_header->p_type),
		program_header->p_offset,
		program_header->p_vaddr,
		program_header->p_paddr,
		program_header->p_filesz,
		program_header->p_memsz,
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

/**
 * read_elf32_be_prog - Convert a 32-bit ELF header from big-endian to host
 * byte order.
 * This function takes a pointer to a 32-bit ELF header big-endian byte order
 * and converts various header fields to the host byte order. It is used to
 * ensure
 * correct interpretation of the header on the host system.
 *
 * @phdr: A pointer to a 32-bit program header in big-endian byte order.
 */
void read_elf32_be_prog(Elf32_Phdr *phdr)
{
	phdr->p_type = my_be32toh(phdr->p_type);
	phdr->p_offset = my_be32toh(phdr->p_offset);
	phdr->p_vaddr = my_be32toh(phdr->p_vaddr);
	phdr->p_paddr = my_be32toh(phdr->p_paddr);
	phdr->p_filesz = my_be32toh(phdr->p_filesz);
	phdr->p_memsz = my_be32toh(phdr->p_memsz);
	phdr->p_flags = my_be32toh(phdr->p_flags);
	phdr->p_align = my_be32toh(phdr->p_align);
}

/**
 * read_elf32_be_header - Convert a 32-bit ELF header from big-endian to host
 * byte order.
 * This function takes a pointer to a 32-bit ELF header big-endian byte order
 * and converts various header fields to the host byte order. It is used to
 * ensure
 * correct interpretation of the header on the host system.
 *
 * @ehdr: A pointer to a 32-bit ELF header in big-endian byte order.
 */
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
