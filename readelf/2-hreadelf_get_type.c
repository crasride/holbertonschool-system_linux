#include "hreadelf.h"


/**
* getElfTypeName - Get a string representation of an ELF file type.
* This function takes a 16-bit ELF file type (`e_type`) as input and returns a
* human-readable string describing that type. It is used interpret the purpose
* or category of an ELF file based on its type.
*
* @e_type: The 16-bit ELF file type.
* Return:       A string describing the ELF file type.
*/
const char *getElfTypeName(uint16_t e_type)
{
	switch (e_type)
	{
	case ET_NONE: return "NONE";
	case ET_REL: return "REL";
	case ET_EXEC: return "EXEC (Executable file)";
	case ET_DYN: return "DYN (Shared object file)";
	case ET_CORE: return "CORE";
	default: return "UNKNOWN";
	}
}

/**
* getProgramHeaderTypeName32 - Get a string representation of a 32-bit ELF
* program header type.
* This function takes a 32-bit ELF program header type (`p_type`) as input and
* returns a human-readable string describing that type. It is useful for
* converting the numeric program header type into a more understandable string
* representation.
*
* @p_type:The 32-bit ELF program header type.
* Return: A string describing the program header type.
*/
const char *getProgramHeaderTypeName32(uint32_t p_type)
{
	switch (p_type)
	{
	case PT_NULL: return "NULL";
	case PT_LOAD: return "LOAD";
	case PT_DYNAMIC: return "DYNAMIC";
	case PT_INTERP: return "INTERP";
	case PT_NOTE: return "NOTE";
	case PT_SHLIB: return "SHLIB";
	case PT_PHDR: return "PHDR";
	case PT_TLS: return "TLS";
	case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
	case PT_GNU_STACK: return "GNU_STACK";
	case PT_GNU_RELRO: return "GNU_RELRO";
	case 0x6464E550:return ("LOOS+464e550");
	case PT_IA_64_UNWIND: return "PT_SUNW_UNWIND";
	default: return "UNKNOWN";
	}
}

/**
* getProgramHeaderTypeName64 - Get a string representation of a 64-bit ELF
* program header type.
* This function takes a 64-bit ELF program header type (`p_type`) as input and
* returns a human-readable string describing that type. It is useful for
* converting the numeric program header type into a more understandable string
* representation.
*
* @p_type:The 64-bit ELF program header type.
* Return: A string describing the program header type.
*/
const char *getProgramHeaderTypeName64(uint64_t p_type)
{
	switch (p_type)
	{
	case PT_NULL: return "NULL";
	case PT_LOAD: return "LOAD";
	case PT_DYNAMIC: return "DYNAMIC";
	case PT_INTERP: return "INTERP";
	case PT_NOTE: return "NOTE";
	case PT_SHLIB: return "SHLIB";
	case PT_PHDR: return "PHDR";
	case PT_TLS: return "TLS";
	case PT_GNU_EH_FRAME: return "GNU_EH_FRAME";
	case PT_GNU_STACK: return "GNU_STACK";
	case PT_GNU_RELRO: return "GNU_RELRO";
	default: return "UNKNOWN";
	}
}


