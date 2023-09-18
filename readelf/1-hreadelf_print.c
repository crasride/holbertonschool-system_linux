#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "hreadelf.h"


void printKeyToFlags_32bits(void)
{
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude),");
	printf(" x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific),");
	printf(" p (processor specific)\n");
}

void printKeyToFlags_64bits(void)
{
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings),");
	printf(" l (large)\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude),");
	printf(" x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific),");
	printf(" p (processor specific)\n");
}

void print_Section_Info_32bits(int index, Elf32_Shdr section_header,
								char *name)
{
	const char *flags = getSectionFlags(section_header.sh_flags);

	printf("  [%2d] %-17s %-15s %08x %06x %06x %02x %s %2d %3d %2d\n",
		index, name, getSectionTypeName(section_header.sh_type),
		section_header.sh_addr, section_header.sh_offset,
		section_header.sh_size, section_header.sh_entsize, flags,
		section_header.sh_link, section_header.sh_info,
		section_header.sh_addralign);
}

void print_Section_Info_64bits(int index, Elf64_Shdr section_header,
								char *name)
{
	const char *flags = getSectionFlags(section_header.sh_flags);

	printf("  [%2d] %-17s %-15s %016lx %06lx %06lx %02lx %s %2d %3d %2ld\n",
		index, name, getSectionTypeName(section_header.sh_type),
		section_header.sh_addr, section_header.sh_offset,
		section_header.sh_size, section_header.sh_entsize, flags,
		section_header.sh_link, section_header.sh_info,
		section_header.sh_addralign);
}
