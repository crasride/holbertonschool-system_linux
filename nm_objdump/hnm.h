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

typedef struct header32
{
	Elf32_Ehdr ehdr;
} Elf32_Header;

typedef struct header64
{
	Elf64_Ehdr ehdr;
} Elf64_Header;

typedef struct
{
	uint32_t st_name;
	uint8_t st_info;
	uint8_t st_other;
	uint16_t st_shndx;
	uint32_t st_value;
	uint32_t st_size;
} Elf_Symbol;

/* Funtions Task 0*/

int analyze_file(const char *filename);
int analyze_64bit_elf(Elf64_Ehdr *ehdr);
int analyze_32bit_elf(Elf32_Ehdr *ehdr);

#endif /* _HNM_ */
