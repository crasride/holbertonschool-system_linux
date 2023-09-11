#include <elf.h>
#include "hreadelf.h"


const char *get_osabi_name(uint8_t osabi)
{
	switch (osabi)
	{
		case ELFOSABI_NONE:
			return ("UNIX - System V");
		case ELFOSABI_NETBSD:
			return ("UNIX - NetBSD");
		case ELFOSABI_SOLARIS:
			return ("UNIX - Solaris");
		default:
			return ("<unknown: 53>");
	}
}

const char *get_osabi_version(uint8_t version)
{
	switch (version)
	{
		case ELFOSABI_NONE:
			return ("0");
		case ELFOSABI_NETBSD:
			return ("0");
		case ELFOSABI_SOLARIS:
			return ("1");
		default:
			return ("0");
	}
}
const char *get_osabi_type(uint16_t type)
{
	switch (type)
	{
		case ET_NONE:
			return ("An unknown type");
		case ET_DYN:
			return ("DYN (Shared object file)");
		case ET_EXEC:
			return ("EXEC (Executable file)");
		case ET_REL:
			return ("REL (Relocatable file)");
		case ET_CORE:
			return ("A core file");
		default:
			return ("EXEC (Executable file)");
	}
}

const char *get_osabi_machine(uint16_t machine)
{
	switch (machine)
	{
		case EM_SPARC:
			return ("Sparc");
		case EM_SPARCV9:
			return ("Sparc");
		case EM_SPARC32PLUS:
			return ("Sparc");
		case EM_X86_64:
			return ("Advanced Micro Devices X86-64");
		case EM_386:
			return ("Intel 80386");
		case EM_NONE:
			return ("An unknown machine");
		default:
			return ("Sparc");
	}
}

const char *get_osabi_data(uint8_t data)
{
	switch (data)
	{
		case ELFDATA2LSB:
			return ("2's complement, little endian");
		case ELFDATA2MSB:
			return ("2's complement, big endian");
		case ELFDATANONE:
			return ("Unknown data format");
		default:
			return ("unknown data");
	}
}
