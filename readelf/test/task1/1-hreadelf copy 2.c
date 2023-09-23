#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "hreadelf.h"


int main(int argc, char *argv[]) {
    FILE *file = NULL;
    Elf64_Ehdr elf_header;
    int is_little_endian = 0;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s elf_filename\n", argv[0]);
        exit(1);
    }

    file = fopen(argv[1], "rb");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    // Read the ELF header to determine endianness
    fread(&elf_header, sizeof(Elf64_Ehdr), 1, file);

    if (elf_header.e_ident[EI_DATA] == ELFDATA2LSB) {
        printf("Little-endian ELF\n");
        is_little_endian = 1;
    } else if (elf_header.e_ident[EI_DATA] == ELFDATA2MSB) {
        printf("Big-endian ELF\n");
    } else {
        fprintf(stderr, "Unknown endianness\n");
        fclose(file);
        exit(1);
    }

    if (elf_header.e_ident[EI_CLASS] == ELFCLASS32) {
        printf("32-bit ELF\n");
    } else if (elf_header.e_ident[EI_CLASS] == ELFCLASS64) {
        printf("64-bit ELF\n");
    } else {
        fprintf(stderr, "Unknown ELF class\n");
        fclose(file);
        exit(1);
    }

    // Seek to the section header table
    fseek(file, is_little_endian ? elf_header.e_shoff : elf_header.e_shoff, SEEK_SET);

    // Read and print section headers
    printf("Section Headers:\n");

    for (int i = 0; i < elf_header.e_shnum; i++) {
        if (elf_header.e_ident[EI_CLASS] == ELFCLASS32) {
            Elf32_Shdr section_header;
            fread(&section_header, sizeof(Elf32_Shdr), 1, file);
            printf("Name: %d\n", section_header.sh_name); // You can extract the name from the string table
            // Print other section header information as needed
        } else if (elf_header.e_ident[EI_CLASS] == ELFCLASS64) {
            Elf64_Shdr section_header;
            fread(&section_header, sizeof(Elf64_Shdr), 1, file);
            printf("Name: %d\n", section_header.sh_name); // You can extract the name from the string table
            // Print other section header information as needed
        }
    }

    fclose(file);
    return 0;
}










void printElf32SectionHeader(const MyElf32_Shdr *section_header)
{
printf("  [ %2u] %-16s %-8s %08x %08x %08x %2u %2u %2u %08x\n",
	section_header->sh_name, "NULL", "NULL", section_header->sh_addr,
	section_header->sh_offset, section_header->sh_size,
	section_header->sh_type, section_header->sh_flags,
	section_header->sh_entsize, section_header->sh_link);
}

void printElf64SectionHeader(const MyElf64_Shdr *section_header)
{
printf("  [ %2u] %-16s %-8s %016lx %016lx %016lx %2u %2lu %2lu %016x\n",
	section_header->sh_name, "NULL", "NULL", section_header->sh_addr,
	section_header->sh_offset, section_header->sh_size,
	section_header->sh_type, section_header->sh_flags,
	section_header->sh_entsize, section_header->sh_link);
}

