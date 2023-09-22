#include "hreadelf.h"


/**
 * main - entry point of make 0
 * @argc: count of arguments
 * @argv: array of arguments
 * Return: 0 success, 1 otherwise
 */
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
		return (1);
	}

	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		perror("No se puede abrir el archivo");
		return (1);
	}

	/* Leer el encabezado ELF principal */
	fread(&elf_header, sizeof(ElfHeader), 1, file);
	/* Verificar si es un archivo ELF de 32 o 64 bits */
	if (elf_header.ehdr.ehdr32.e_ident[EI_CLASS] == ELFCLASS32)
	{

		if (elf_header.ehdr.ehdr32.e_phoff == 0)
		{
			printf("\nThere are no program headers in this file.\n");
			return (0);
		}
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
		if (elf_header.ehdr.ehdr64.e_phoff == 0)
		{
			printf("\nThere are no program headers in this file.\n");
			return (0);
		}
		is_32bit = 0;
	}
	else
	{
		fprintf(stderr, "El archivo ELF tiene una clase desconocida.\n");
		fclose(file);
		return (1);
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
					return (1);
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
					return (1);
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
	return (0);
}


/**
* createSectionToSegmentMapping64 - Create and print a mapping of sections
* to segments in an ELF file (64-bit).
* This function reads the ELF file generates a mapping of sections to segments.
* It then prints this mapping to the standard output.
*
* @file:A pointer to the ELF file.
* @elf_header: elf_header A pointer to the ELF header structure.
* @is_32bit: A flag indicating whether the ELF file is 32-bit (1) or 64-bit (0).
*/
void createSectionToSegmentMapping64(FILE *file, ElfHeader *elf_header,
									int is_32bit)
{
	int i, j;
	Elf64_Shdr shstrtab_header;
	Elf64_Shdr section_headers[MAX_INTERP_SIZE];
	char *shstrtab = NULL;
	char sections[MAX_INTERP_SIZE] = "";

	SectionToSegmentMapping *mapping = (SectionToSegmentMapping *)malloc(elf_header->ehdr.ehdr64.e_phnum * sizeof(SectionToSegmentMapping));

	for (i = 0; i < elf_header->ehdr.ehdr64.e_phnum; i++)
	{
		mapping[i].segment_number = i;
		strcpy(mapping[i].sections, "");
	}

	/* Obtener la tabla de cadenas de secciones */
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->ehdr.ehdr64.e_shoff) + elf_header->ehdr.ehdr64.e_shstrndx * sizeof(Elf64_Shdr), SEEK_SET);
	fread(&shstrtab_header, sizeof(Elf64_Shdr), 1, file);

	shstrtab = (char *)malloc(shstrtab_header.sh_size);
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
			/* Omitir la asignación de estas secciones específicas */
			if (strcmp(section_name, ".gnu_debuglink") == 0 || strcmp(section_name, ".shstrtab") == 0 || strcmp(section_name, ".tm_clone_table") == 0)
			{
				continue;
			}

			if (section_header.sh_addr >= program_header.p_vaddr && section_header.sh_addr + section_header.sh_size <= program_header.p_vaddr + program_header.p_memsz)
			{
				if (strlen(sections) > 0)
				{
					strcat(sections, " ");
				}
				strcat(sections, section_name);
			}
		}
		if (strlen(sections) > 0)
		{
			strcpy(mapping[i].sections, sections);
		}
	}

	free(shstrtab);
	/* Imprimir la tabla de mapeo de secciones a segmentos */
	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	for (i = 0; i < elf_header->ehdr.ehdr64.e_phnum; i++)
	{
		if (strlen(mapping[i].sections) > 0)
		{
			printf("   %02d     %s \n", mapping[i].segment_number, mapping[i].sections);
		}
		else
		{
			printf("   %02d     \n", mapping[i].segment_number);
		}
	}
	free(mapping);
}

/**
* createSectionToSegmentMapping32 - Create and print a mapping of sections
* to segments in an ELF file (32-bit).
* This function reads the ELF file generates a mapping of sections to segments.
* It then prints this mapping to the standard output.
*
* @file:A pointer to the ELF file.
* @elf_header: elf_header A pointer to the ELF header structure.
* @is_32bit: A flag indicating whether the ELF file is 32-bit (1) or 64-bit (0).
*/
void createSectionToSegmentMapping32(FILE *file, ElfHeader *elf_header,
									int is_32bit)
{
	int i, j;
	Elf32_Shdr shstrtab_header;
	Elf32_Shdr section_headers[MAX_INTERP_SIZE];
	char *shstrtab = NULL;
	char sections[MAX_INTERP_SIZE] = "";

	SectionToSegmentMapping *mapping = (SectionToSegmentMapping *)malloc(elf_header->ehdr.ehdr32.e_phnum * sizeof(SectionToSegmentMapping));

	for (i = 0; i < elf_header->ehdr.ehdr32.e_phnum; i++)
	{
		mapping[i].segment_number = i;
		strcpy(mapping[i].sections, "");
	}

	/* Obtener la tabla de cadenas de secciones */
	fseek(file, (is_32bit ? elf_header->ehdr.ehdr32.e_shoff : elf_header->ehdr.ehdr64.e_shoff) + elf_header->ehdr.ehdr32.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET);

	fread(&shstrtab_header, sizeof(Elf32_Shdr), 1, file);
	if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
		read_elf32_be_section(&shstrtab_header);

	shstrtab = (char *)malloc(shstrtab_header.sh_size);
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
		if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
			read_elf32_be_prog(&program_header);
		strcpy(sections, "");

		for (j = 0; j < elf_header->ehdr.ehdr32.e_shnum; j++)
		{
			Elf32_Shdr section_header;
			const char *section_name;

			section_header = section_headers[j];
			if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2MSB)
				read_elf32_be_section(&section_header);
			/* Obtener el nombre de la sección utilizando la tabla de cadenas de secciones */
			section_name = shstrtab + section_header.sh_name;
			/* Omitir la asignación de estas secciones específicas */
			if (elf_header->ehdr.ehdr32.e_ident[EI_DATA] == ELFDATA2LSB)
			{
				if (strcmp(section_name, ".gnu_debuglink") == 0 || strcmp(section_name, ".shstrtab") == 0 || strcmp(section_name, ".tm_clone_table") == 0)
				{
					continue;
				}
			}

			if (section_header.sh_addr >= program_header.p_vaddr && section_header.sh_addr + section_header.sh_size <= program_header.p_vaddr + program_header.p_memsz)
			{
				if (strlen(sections) > 0)
				{
					strcat(sections, " ");
				}
				strcat(sections, section_name);
			}

		}
		if (strlen(sections) > 0)
		{
			strcpy(mapping[i].sections, sections);
		}
	}
	free(shstrtab);
	/* Imprimir la tabla de mapeo de secciones a segmentos */
	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	for (i = 0; i < elf_header->ehdr.ehdr32.e_phnum; i++)
	{
		if (strlen(mapping[i].sections) > 0)
		{
			printf("   %02d     %s \n", mapping[i].segment_number, mapping[i].sections);
		}
		else
		{
			printf("   %02d     \n", mapping[i].segment_number);
		}
	}
	free(mapping);
}
