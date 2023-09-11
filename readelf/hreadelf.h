#ifndef HELF_H
#define HELF_H

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	Elf32_Ehdr ehdr;
} Elf32_Header;

typedef struct
{
	Elf64_Ehdr ehdr;
} Elf64_Header;


#endif /* HELF_H */
