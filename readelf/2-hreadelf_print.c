#include "hreadelf.h"


/**
* print_interpreter_info - Print information about the program interpreter.
*
* This function prints information about the program interpreter requested by
* an ELF file. The program interpreter is used in dynamically linked executables
* to load and execute shared libraries.
*
* @interp: A string containing the path to the program interpreter.
*/


void print_interpreter_info(const char *interp)
{
	printf("      [Requesting program interpreter: %s]\n", interp);
}

/**
* print_elf_info - Print information about an ELF file.
*
* This function prints information about an ELF file, including its type, entry
* point, program header details. It supports both 32-bit and 64-bit ELF files.
*
* @elf_header: A pointer to an `ElfHeader` structure containing ELF header data.
* @is_32bit: A flag indicating whether the ELF file is 32-bit (1) or 64-bit (0).
*/
void print_elf_info(ElfHeader *elf_header, int is_32bit)
{
	printf("\nElf file type is %s\n", getElfTypeName(is_32bit ? elf_header->
	ehdr.ehdr32.e_type : elf_header->ehdr.ehdr64.e_type));

	if (is_32bit)
	{
		printf("Entry point 0x%x\n", elf_header->ehdr.ehdr32.e_entry);
		printf("There are %d program headers, starting at offset %ld\n\n",
		is_32bit ? elf_header->ehdr.ehdr32.e_phnum : elf_header->
		ehdr.ehdr64.e_phnum,
		is_32bit ? (long)elf_header->ehdr.ehdr32.e_phoff : (long)elf_header->
		ehdr.ehdr64.e_phoff);

	printf("Program Headers:\n");
	printf("  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align\n");

	}
	else
	{
		printf("Entry point 0x%1lx\n", elf_header->ehdr.ehdr64.e_entry);
		printf("There are %d program headers, starting at offset %ld\n\n",
		is_32bit ? elf_header->ehdr.ehdr32.e_phnum : elf_header->
		ehdr.ehdr64.e_phnum,
		is_32bit ? (long)elf_header->ehdr.ehdr32.e_phoff : (long)elf_header->
		ehdr.ehdr64.e_phoff);

	printf("Program Headers:\n");
	printf("  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align\n");
	}
}
