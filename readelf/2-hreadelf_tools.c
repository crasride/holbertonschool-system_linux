#include "hreadelf.h"


/**
* print_program_header_info_32 - Print information about a 64-bit ELF program
* header.
* This function takes a pointer to a 32-bit ELF program header and prints its
* fields in a human-readable format. It displays details such as the program
* header type,offset, virtual address, physical address, file size,
* memory size, permissions (read,write, execute), and alignment.
*
* @program_header: A pointer to a 32-bit ELF program header.
*/
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


/**
* print_program_header_info_64 - Print information about a 64-bit ELF program
* header.
* This function takes a pointer to a 64-bit ELF program header and prints its
* fields in a human-readable format. It displays details such as the program
* header type,offset, virtual address, physical address, file size,
* memory size, permissions (read,write, execute), and alignment.
*
*@program_header: A pointer to a 64-bit ELF program header.
*/
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

/**
* read_elf32_be_section - Convert a 32-bit ELF section header from big-endian
* to host byte order.
* This function takes a pointer to a 32-bit ELF section header in big-endian
* byte order
* and converts various section header fields to the host byte order.It is used
* to ensure
* correct interpretation of the section header on the host system.
*
* @section_header32: pointer to a 32-bit ELF section header in big-endian
* byte order.
*/
void read_elf32_be_section(Elf32_Shdr *section_header32)
{
	section_header32->sh_name = my_be32toh(section_header32->sh_name);
	section_header32->sh_type = my_be32toh(section_header32->sh_type);
	section_header32->sh_addr = my_be32toh(section_header32->sh_addr);
	section_header32->sh_offset = my_be32toh(section_header32->sh_offset);
	section_header32->sh_size = my_be32toh(section_header32->sh_size);
	section_header32->sh_entsize = my_be32toh(section_header32->sh_entsize);
	section_header32->sh_flags = my_be32toh(section_header32->sh_flags);
	section_header32->sh_link = my_be32toh(section_header32->sh_link);
	section_header32->sh_info = my_be32toh(section_header32->sh_info);
	section_header32->sh_addralign = my_be32toh(section_header32->sh_addralign);
}
