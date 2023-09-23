#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include "hreadelf.h"

typedef struct {
    uint32_t  p_type;
    uint32_t  p_offset;
    uint32_t  p_vaddr;
    uint32_t  p_paddr;
    uint32_t  p_filesz;
    uint32_t  p_memsz;
    uint32_t  p_flags;
    uint32_t  p_align;
} MyElf32_Phdr;

typedef struct {
    uint64_t  p_type;
    uint64_t  p_flags;
    uint64_t  p_offset;
    uint64_t  p_vaddr;
    uint64_t  p_paddr;
    uint64_t  p_filesz;
    uint64_t  p_memsz;
    uint64_t  p_align;
} MyElf64_Phdr;


int main(int argc, char *argv[])
{
    FILE *file = NULL;
    int index = 0;
    Elf32_Ehdr elf_header32;
    Elf64_Ehdr elf_header64;
    off_t section_table_offset;
    Elf32_Shdr section_header32;
    Elf64_Shdr section_header64;
    MyElf32_Phdr program_header32;
    MyElf64_Phdr program_header64;
    char* SectNames = NULL;
    int is_32bit = 0;

    if (argc != 2)
    {
        return (EXIT_SUCCESS);
    }
    file = fopen(argv[1], "rb");
    if (file == NULL)
    {
        perror("No se puede abrir el archivo");
        return 1;
    }

    /* Leer el encabezado ELF principal */
    fread(&elf_header32, sizeof(Elf32_Ehdr), 1, file);

    /* Verificar si es un archivo ELF de 32 bits */
    if (elf_header32.e_ident[EI_CLASS] == ELFCLASS32)
    {
        is_32bit = 1;

        /* Detectar el endianness del archivo ELF de 32 bits */
        if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
        {
            read_elf32_be_header(&elf_header32);
        }
        else if (elf_header32.e_ident[EI_DATA] == ELFDATA2LSB)
        {
            // Si es little-endian, puedes seguir con el código actual.
        }
        else
        {
            printf("Formato de datos ELF de 32 bits no reconocido.\n");
            return 1;
        }
    }
    else
    {
        /* Retroceder al principio del archivo si no es un archivo de 32 bits */
        fseek(file, 0, SEEK_SET);
        /* Leer el encabezado ELF de 64 bits */
        fread(&elf_header64, sizeof(Elf64_Ehdr), 1, file);
    }

    /* Leer la posición de la tabla de secciones */
    section_table_offset = is_32bit ? elf_header32.e_shoff : elf_header64.e_shoff;

    /* Create sectHdr and SectNames */
    if (is_32bit)
    {
        if (elf_header32.e_ident[EI_DATA] == ELFDATA2LSB)
        {
            read_elf32_be_section(&section_header32);
            fseek(file, elf_header32.e_shoff + elf_header32.e_shstrndx * elf_header32.e_shentsize, SEEK_SET);
            SectNames = get_section_name32(section_header32, file);
        }

        if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
        {
            read_elf32_be_section(&section_header32);
            fseek(file, elf_header32.e_shoff + elf_header32.e_shstrndx * elf_header32.e_shentsize, SEEK_SET);
            SectNames = get_section_name32_big(section_header32, file);
        }

        /* Leer el encabezado del programa ELF de 32 bits */
        fseek(file, elf_header32.e_phoff, SEEK_SET);
        fread(&program_header32, sizeof(MyElf32_Phdr), 1, file);
    }
    else
    {
        fseek(file, elf_header64.e_shoff + elf_header64.e_shstrndx * elf_header64.e_shentsize, SEEK_SET);
        SectNames = get_section_name64(section_header64, file);

        /* Leer el encabezado del programa ELF de 64 bits */
        fseek(file, elf_header64.e_phoff, SEEK_SET);
        fread(&program_header64, sizeof(MyElf64_Phdr), 1, file);
    }

    /* Imprimir información del encabezado ELF principal */
    printf("Elf file type is %s\n", getElfTypeName(is_32bit ? elf_header32.e_type : elf_header64.e_type));

    /* Imprimir información sobre la tabla de secciones */
    printf("There are %u section headers, starting at offset 0x%lx:\n\n",
        is_32bit ? elf_header32.e_shnum : elf_header64.e_shnum,
        section_table_offset);

	    /* Ubicar la tabla de secciones y encabezado de sección */
    if (is_32bit)
    {
        fseek(file, elf_header32.e_shoff, SEEK_SET);
        printf("Section Headers:\n");
        printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al\n");
    }
    else
    {
        fseek(file, elf_header64.e_shoff, SEEK_SET);
        printf("Section Headers:\n");
        printf("  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");
    }

    if (is_32bit)
    {
        Elf32_Shdr section_header32;

        for (index = 0; index < elf_header32.e_shnum; index++)
        {
            char* name = "";

            fread(&section_header32, sizeof(Elf32_Shdr), 1, file);

            if (elf_header32.e_ident[EI_DATA] == ELFDATA2MSB)
                read_elf32_be_section(&section_header32);

            if (section_header32.sh_name)
                name = SectNames + section_header32.sh_name;

            /* Llamar a la función para imprimir la información de la sección de 32 bits */
            print_Section_Info_32bits(index, section_header32, name);
        }

        printKeyToFlags_32bits();
    }
    else
    {
        Elf64_Shdr section_header64;

        for (index = 0; index < elf_header64.e_shnum; index++)
        {
            char* name = "";
            fread(&section_header64, sizeof(Elf64_Shdr), 1, file);

            if (section_header64.sh_name)
                name = SectNames + section_header64.sh_name;

            /* Llamar a la función para imprimir la información de la sección de 64 bits */
            print_Section_Info_64bits(index, section_header64, name);
        }

        printKeyToFlags_64bits();
    }

    /* Imprimir información del encabezado del programa ELF */
    printf("\nProgram Headers:\n");

    if (is_32bit)
    {
        /* Imprimir información del encabezado del programa ELF de 32 bits */
        printf("  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align\n");

        /* Llamar a la función para imprimir la información del encabezado del programa de 32 bits */
        print_Program_Header_Info_32bits(program_header32);
    }
    else
    {
        /* Imprimir información del encabezado del programa ELF de 64 bits */
        printf("  Type           Offset   VirtAddr           PhysAddr           FileSiz MemSiz  Flg Align\n");

        /* Llamar a la función para imprimir la información del encabezado del programa de 64 bits */
        print_Program_Header_Info_64bits(program_header64);
    }

    fclose(file);
    return 0;
}


void print_Program_Header_Info_32bits(MyElf32_Phdr program_header)
{
    printf("  %-14s 0x%06x 0x%08x 0x%08x 0x%05x 0x%05x %c%c%c 0x%04x\n",
           getProgramHeaderTypeName(program_header.p_type),
           program_header.p_offset,
           program_header.p_vaddr,
           program_header.p_paddr,
           program_header.p_filesz,
           program_header.p_memsz,
           (program_header.p_flags & PF_R) ? 'R' : '-',
           (program_header.p_flags & PF_W) ? 'W' : '-',
           (program_header.p_flags & PF_X) ? 'E' : '-',
           program_header.p_align);
}

void print_Program_Header_Info_64bits(MyElf64_Phdr program_header)
{
    printf("  %-14s 0x%06lx 0x%016lx 0x%016lx 0x%06lx 0x%06lx %c%c%c 0x%04lx\n",
           getProgramHeaderTypeName(program_header.p_type),
           program_header.p_offset,
           program_header.p_vaddr,
           program_header.p_paddr,
           program_header.p_filesz,
           program_header.p_memsz,
           (program_header.p_flags & PF_R) ? 'R' : '-',
           (program_header.p_flags & PF_W) ? 'W' : '-',
           (program_header.p_flags & PF_X) ? 'E' : '-',
           program_header.p_align);
}


const char *getElfTypeName(uint16_t e_type)
{
    switch (e_type)
    {
    case ET_NONE: return "NONE (None)";
    case ET_REL: return "REL (Relocatable file)";
    case ET_EXEC: return "EXEC (Executable file)";
    case ET_DYN: return "DYN (Shared object file)";
    case ET_CORE: return "CORE (Core file)";
    default: return "UNKNOWN";
    }
}

const char *getProgramHeaderTypeName(uint32_t p_type)
{
    switch (p_type)
    {
    case PT_NULL: return "NULL";
    case PT_LOAD: return "LOAD (Loadable segment)";
    case PT_DYNAMIC: return "DYNAMIC (Dynamic linking information)";
    case PT_INTERP: return "INTERP (Interpreter path name)";
    case PT_NOTE: return "NOTE (Note)";
    case PT_SHLIB: return "SHLIB (Reserved)";
    case PT_PHDR: return "PHDR (Program header table)";
    case PT_TLS: return "TLS (Thread-local storage)";
    case PT_GNU_EH_FRAME: return "GNU_EH_FRAME (GCC .eh_frame_hdr segment)";
    case PT_GNU_STACK: return "GNU_STACK (GNU-style stack segment)";
    case PT_GNU_RELRO: return "GNU_RELRO (Read-only after relocation)";
    default: return "UNKNOWN";
    }
}

void read_elf32_be_program_header(MyElf32_Phdr *program_header32)
{
    program_header32->p_type = my_be32toh(program_header32->p_type);
    program_header32->p_offset = my_be32toh(program_header32->p_offset);
    program_header32->p_vaddr = my_be32toh(program_header32->p_vaddr);
    program_header32->p_paddr = my_be32toh(program_header32->p_paddr);
    program_header32->p_filesz = my_be32toh(program_header32->p_filesz);
    program_header32->p_memsz = my_be32toh(program_header32->p_memsz);
    program_header32->p_flags = my_be32toh(program_header32->p_flags);
    program_header32->p_align = my_be32toh(program_header32->p_align);
}

uint32_t my_be32toh(uint32_t value)
{
    return (((value >> 24) & 0xFF) |
            ((value >> 8) & 0xFF00) |
            ((value & 0xFF00) << 8) |
            ((value & 0xFF) << 24));
}

