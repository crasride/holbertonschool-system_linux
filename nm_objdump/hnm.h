#ifndef _HNM_
#define _HNM_

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

/* struct for 32 bits */
typedef struct header32
{
	Elf32_Ehdr ehdr;
} Elf32_Header;

typedef struct Shdr32
{
	Elf32_Shdr shdr;
} MyElf32_Shdr;

typedef struct
{
	Elf32_Word st_name;     /* Symbol name (index to string table) */
	Elf32_Addr st_value;    /* Symbol value */
	Elf32_Word st_size;     /* Symbol size */
	unsigned char st_info;  /* Symbol type and binding */
	unsigned char st_other; /* Symbol visibility */
	Elf32_Half st_shndx;    /* Section index */
} MyElf32_Sym;

/* struct for 64 bits */
typedef struct header64
{
	Elf64_Ehdr ehdr;
} Elf64_Header;

typedef struct Shdr64
{
	Elf64_Shdr shdr;
} MyElf64_Shdr;

typedef struct
{
	Elf64_Word st_name;    /* Symbol name (index to string table) */
	unsigned char st_info; /* Symbol type and binding */
	unsigned char st_other; /* Symbol visibility */
	Elf64_Half st_shndx;   /* Section index */
	Elf64_Addr st_value;   /* Symbol value */
	Elf64_Xword st_size;   /* Symbol size */
} MyElf64_Sym;

/* Funtions Task 0*/

int analyze_file(const char *filename);
int analyze_64bit_elf(Elf64_Ehdr *ehdr);
/* int analyze_32bit_elf(Elf32_Ehdr *ehdr); */
void process_symbols_32bit(Elf32_Ehdr *ehdr, void *map);
int analyze_32bit_elf(Elf32_Ehdr *ehdr, void *map);

#endif /* _HNM_ */
