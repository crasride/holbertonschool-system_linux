#include <elf.h>
#include "hreadelf.h"

/**
 * get_osabi_name - Retrieve the name of an ELF OS ABI.
 * @osabi: The ELF OS ABI value.
 *
 * Return: A string describing the ELF OS ABI.
 *
 * This returns a human-readable description an ELF OS ABI based on its value.
 * It supports common OS ABI values such as UNIX - System V, UNIX - NetBSD,UNIX
 * - Solaris, and provides an unknown value if the OS ABI is not recognized.
 */

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

/**
 * get_osabi_version - Retrieve the version of an ELF OS ABI.
 * @version: The ELF OS ABI version value.
 *
 * Return: A string describing the ELF OS ABI version.
 *
 * This returns a human-readable description of the version of an ELF OS ABI
 * based on its value.
 * It supports common version values, such as 0 and 1, and provides a default
 * value of "0" for unknown versions.
 */

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

/**
 * get_osabi_type - Retrieve the type of an ELF file.
 * @type: The ELF file type value.
 *
 * Return: A string describing the type of the ELF file.
 *
 * This returns a human-readable description of the type of an ELF file based
 * on its value.
 * Supported types include An unknown type, DYN (Shared object file),
 * EXEC (Executable file), REL (Relocatable file), and A core file.
 * If the type is not recognized, it defaults to "EXEC (Executable file)".
 */

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

/**
 * get_osabi_machine - Retrieve the machine architecture of an ELF file.
 * @machine: The ELF machine architecture value.
 *
 * Return: A string describing the ELF machine architecture.
 *
 * This function returns a human-readable description the machine architecture
 * of an ELF file based on its value.
 * Supported architectures include Sparc, Advanced Micro Devices X86-64,
 * Intel 80386, and others.
 * If the architecture is not recognized, it defaults to "Sparc".
 */

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

/**
 * get_osabi_data - Retrieve the data format of an ELF file.
 * @data: The ELF data format value.
 *
 * Return: A string describing the ELF data format.
 *
 * This function returns a human-readable description of the data format of an
 * ELF file based on its value.
 * Supported data formats include 2's complement, little endian,2's complement,
 * big endian, and an unknown data format.
 * If the data format is not recognized, it defaults to "unknown data".
 */

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
