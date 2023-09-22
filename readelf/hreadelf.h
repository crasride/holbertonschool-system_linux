#ifndef HELF_H
#define HELF_H

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


/* ------------------- Task 0-hreadelf.c ----------------------------------*/
/**
* struct header32 - Represents a 32-bit ELF header.
* Structure represents a 32-bit ELF (Executable and Linkable Format) header.
* It includes a member named 'ehdr' of type 'Elf32_Ehdr', which holds the main
* ELF header information.
* ELF header provides essential information about the ELF file format, such
* as its class (32-bit), data encoding, file type, machine architecture, entry
* point, program header and section header information, and more.
* @ehdr: structure is typically used for parsing and working with 32-bit ELF
*/
typedef struct header32
{
	Elf32_Ehdr ehdr;
} Elf32_Header;

/**
* struct header64 - Represents a 64-bit ELF header.
* Structure represents a 64-bit ELF (Executable and Linkable Format) header.
* It includes a member named 'ehdr' of type 'Elf64_Ehdr', which holds the main
* ELF header information.
* ELF header provides essential information about the ELF file format, such
* as its class (64-bit), data encoding, file type, machine architecture, entry
* point, program header and section header information, and more.
* @ehdr: structure is typically used for parsing and working with 64-bit ELF
*/
typedef struct header64
{
	Elf64_Ehdr ehdr;
} Elf64_Header;

/* Functions 0-hreadelf_tools.c */
const char *get_osabi_name(uint8_t osabi);
const char *get_osabi_version(uint8_t version);
const char *get_osabi_type(uint16_t type);
const char *get_osabi_machine(uint16_t machine);
const char *get_osabi_data(uint8_t data);

/* Functions 0-hreadelf_convert.c */
uint16_t my_be16toh(uint16_t value);
uint32_t my_be32toh(uint32_t value);

/* Functions 0-hreadelf.c */
void print_elf32_header(Elf32_Header *elf32);
void print_elf64_header(Elf64_Header *elf64);
void read_elf32_be_header0(Elf32_Header *elf32, FILE *file);
int main(int argc, char *argv[]);

/* ------------------- Task 1-hreadelf.c ----------------------------------*/

/**
* struct Shdr32 - Represents a 32-bit ELF section header entry.
* This structure represents a section header entry in a 32-bit ELF (Executable
* and Linkable Format) file. It stores information about a specific section in
* the ELF file, including its name, type, flags, memory address, file offset,
* size, linkage, additional information, alignment, and entry size.
*
* The section header is a crucial part of the ELF file format, as it defines
* the layout and properties of various sections within the file, such as code,
* data, and symbol tables.
* Structure is typically used for parsing and working with 32-bit ELF
*
* @sh_name: Section name, index in string tbl
* @sh_type: Type of section
* @sh_flags: Miscellaneous section attributes
* @sh_addr: Section virtual addr at execution
* @sh_offset: Section file offset
* @sh_size: Size of section in bytes
* @sh_link: Index of another section
* @sh_info: Additional section information
* @sh_addralign:Section alignment
* @sh_entsize: Entry size if section holds table
*/
typedef struct Shdr32
{
	Elf32_Word sh_name;
	Elf32_Word sh_type;
	Elf32_Word sh_flags;
	Elf32_Addr sh_addr;
	Elf32_Off sh_offset;
	Elf32_Word sh_size;
	Elf32_Word sh_link;
	Elf32_Word sh_info;
	Elf32_Word sh_addralign;
	Elf32_Word sh_entsize;
} MyElf32_Shdr;

/**
* struct Shdr64 - Represents a 64-bit ELF section header entry.
* This structure represents a section header entry in a 32-bit ELF (Executable
* and Linkable Format) file. It stores information about a specific section in
* the ELF file, including its name, type, flags, memory address, file offset,
* size, linkage, additional information, alignment, and entry size.
*
* The section header is a crucial part of the ELF file format, as it defines
* the layout and properties of various sections within the file, such as code,
* data, and symbol tables.
* Structure is typically used for parsing and working with 64-bit ELF.
*
* @sh_name: Section name, index in string tbl
* @sh_type: Type of section
* @sh_flags: Miscellaneous section attributes
* @sh_addr: Section virtual addr at execution
* @sh_offset: Section file offset
* @sh_size: Size of section in bytes
* @sh_link: Index of another section
* @sh_info: Additional section information
* @sh_addralign:Section alignment
* @sh_entsize: Entry size if section holds table
*/
typedef struct Shdr64
{
	Elf64_Word sh_name;
	Elf64_Word sh_type;
	Elf64_Xword sh_flags;
	Elf64_Addr sh_addr;
	Elf64_Off sh_offset;
	Elf64_Xword sh_size;
	Elf64_Word sh_link;
	Elf64_Word sh_info;
	Elf64_Xword sh_addralign;
	Elf64_Xword sh_entsize;
} MyElf64_Shdr;


/* Functions 1-hreadelf.c */
void read_elf32_be_section(Elf32_Shdr *section_header32);
void read_elf32_be_header(Elf32_Ehdr *ehdr);
char *set_section_names(int is_32bit, FILE *file, Elf32_Ehdr elf_header32,
						Elf64_Ehdr elf_header64);
void loop_print(int is_32bit, FILE *file, Elf32_Ehdr elf_header32,
				Elf64_Ehdr elf_header64, char *section_names);

/* Functions 1-hreadelf_print.c*/
void printKeyToFlags_32bits(void);
void printKeyToFlags_64bits(void);
void print_Section_Info_32bits(int index, Elf32_Shdr section_header,
								char *name);
void print_Section_Info_64bits(int index, Elf64_Shdr section_header,
								char *name);
void print_header(int is_32bit, FILE *file, Elf32_Ehdr elf_header32,
					Elf64_Ehdr elf_header64, off_t section_table_offset);

/* Functions 1-hreadelf_tools.c */
char *get_section_name32(Elf32_Shdr section_header, FILE *file);
char *get_section_name32_big(Elf32_Shdr section_header, FILE *file);
char *get_section_name64(Elf64_Shdr section_header, FILE *file);
const char *getSectionTypeName(unsigned int sh_type);
const char *getSectionFlags(unsigned int sh_flags);

/* ------------------- Task 2-hreadelf.c ----------------------------------*/
#define MAX_INTERP_SIZE 1024

typedef struct
{
	union
	{
		Elf32_Ehdr ehdr32;
		Elf64_Ehdr ehdr64;
	} ehdr;
} ElfHeader;
typedef struct
{
	union
	{
		Elf32_Phdr phdr32;
		Elf64_Phdr phdr64;
	} phdr;
} ElfProgramHeader;


/* estructura para el mapeo de sección a segmento */
#define MAX_SECTIONS 100
#define MAX_SECTION_NAME 100

typedef struct
{
	int segment_number;
	char sections[MAX_INTERP_SIZE];
} SectionToSegmentMapping;


const char *getElfTypeName(uint16_t e_type);
void print_elf_info(ElfHeader *elf_header, int is_32bit);
void print_program_header_info_64(Elf64_Phdr *program_header);
void print_program_header_info_32(Elf32_Phdr *program_header);
const char *getProgramHeaderTypeName32(uint32_t p_type);
const char *getProgramHeaderTypeName64(uint64_t p_type);
void print_interpreter_info(const char *interp);
void read_elf32_be_prog(Elf32_Phdr *phdr);

void createSectionToSegmentMapping64(FILE *file, ElfHeader *elf_header, int is_32bit);
void createSectionToSegmentMapping32(FILE *file, ElfHeader *elf_header, int is_32bit);

#endif /* HELF_H */
