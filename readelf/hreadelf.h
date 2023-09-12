#ifndef HELF_H
#define HELF_H

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef struct
{
	Elf32_Ehdr ehdr;
} Elf32_Header;

typedef struct
{
	Elf64_Ehdr ehdr;
} Elf64_Header;

/* Functions 0-hreadelf_tools.c */
const char *get_osabi_name(uint8_t osabi);
const char *get_osabi_version(uint8_t version);
const char *get_osabi_type(uint16_t type);
const char *get_osabi_machine(uint16_t machine);
const char *get_osabi_data(uint8_t data);

#endif /* HELF_H */
