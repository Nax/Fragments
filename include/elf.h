#ifndef _ELF_H
#define _ELF_H

/* Based on linux elf.h */

#include <stdint.h>

#define EM_NONE     0
#define EM_M32      1
#define EM_SPARC    2
#define EM_386      3
#define EM_68K      4
#define EM_88K      5
#define EM_486      6   /* Perhaps disused */
#define EM_860      7
#define EM_MIPS     8   /* MIPS R3000 (officially, big-endian only) */
#define EM_MIPS_RS3_LE  10  /* MIPS R3000 little-endian */
#define EM_MIPS_RS4_BE  10  /* MIPS R4000 big-endian */
#define EM_PARISC   15  /* HPPA */
#define EM_SPARC32PLUS  18  /* Sun's "v8plus" */
#define EM_PPC      20  /* PowerPC */
#define EM_PPC64    21   /* PowerPC64 */
#define EM_SPU      23  /* Cell BE SPU */
#define EM_SH       42  /* SuperH */
#define EM_SPARCV9  43  /* SPARC v9 64-bit */
#define EM_IA_64    50  /* HP/Intel IA-64 */
#define EM_X86_64   62  /* AMD x86-64 */
#define EM_S390     22  /* IBM S/390 */
#define EM_CRIS     76  /* Axis Communications 32-bit embedded processor */
#define EM_V850     87  /* NEC v850 */
#define EM_M32R     88  /* Renesas M32R */
#define EM_H8_300   46  /* Renesas H8/300,300H,H8S */
#define EM_MN10300  89  /* Panasonic/MEI MN10300, AM33 */
#define EM_BLACKFIN     106     /* ADI Blackfin Processor */
#define EM_FRV      0x5441  /* Fujitsu FR-V */
#define EM_AVR32    0x18ad  /* Atmel AVR32 */

/* 32-bit ELF base types. */
typedef uint32_t    Elf32_Addr;
typedef uint16_t    Elf32_Half;
typedef uint32_t    Elf32_Off;
typedef int32_t     Elf32_Sword;
typedef uint32_t    Elf32_Word;

/* These constants are for the segment types stored in the image headers */
#define PT_NULL         0
#define PT_LOAD         1
#define PT_DYNAMIC      2
#define PT_INTERP       3
#define PT_NOTE         4
#define PT_SHLIB        5
#define PT_PHDR         6
#define PT_TLS          7               /* Thread local storage segment */
#define PT_LOOS         0x60000000      /* OS-specific */
#define PT_HIOS         0x6fffffff      /* OS-specific */
#define PT_LOPROC       0x70000000
#define PT_HIPROC       0x7fffffff
#define PT_GNU_EH_FRAME 0x6474e550

#define PT_GNU_STACK    (PT_LOOS + 0x474e551)

/*
 * Extended Numbering
 *
 * If the real number of program header table entries is larger than
 * or equal to PN_XNUM(0xffff), it is set to sh_info field of the
 * section header at index 0, and PN_XNUM is set to e_phnum
 * field. Otherwise, the section header at index 0 is zero
 * initialized, if it exists.
 *
 * Specifications are available in:
 *
 * - Oracle: Linker and Libraries.
 *   Part No: 817–1984–19, August 2011.
 *   http://docs.oracle.com/cd/E18752_01/pdf/817-1984.pdf
 *
 * - System V ABI AMD64 Architecture Processor Supplement
 *   Draft Version 0.99.4,
 *   January 13, 2010.
 *   http://www.cs.washington.edu/education/courses/cse351/12wi/supp-docs/abi.pdf
 */
#define PN_XNUM 0xffff

/* These constants define the different elf file types */
#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4
#define ET_LOPROC 0xff00
#define ET_HIPROC 0xffff

/* This is the info that is needed to parse the dynamic section of the file */
#define DT_NULL     0
#define DT_NEEDED   1
#define DT_PLTRELSZ 2
#define DT_PLTGOT   3
#define DT_HASH     4
#define DT_STRTAB   5
#define DT_SYMTAB   6
#define DT_RELA     7
#define DT_RELASZ   8
#define DT_RELAENT  9
#define DT_STRSZ    10
#define DT_SYMENT   11
#define DT_INIT     12
#define DT_FINI     13
#define DT_SONAME   14
#define DT_RPATH    15
#define DT_SYMBOLIC 16
#define DT_REL          17
#define DT_RELSZ    18
#define DT_RELENT   19
#define DT_PLTREL   20
#define DT_DEBUG    21
#define DT_TEXTREL  22
#define DT_JMPREL   23
#define DT_ENCODING 32
#define OLD_DT_LOOS 0x60000000
#define DT_LOOS     0x6000000d
#define DT_HIOS     0x6ffff000
#define DT_VALRNGLO 0x6ffffd00
#define DT_VALRNGHI 0x6ffffdff
#define DT_ADDRRNGLO    0x6ffffe00
#define DT_ADDRRNGHI    0x6ffffeff
#define DT_VERSYM   0x6ffffff0
#define DT_RELACOUNT    0x6ffffff9
#define DT_RELCOUNT 0x6ffffffa
#define DT_FLAGS_1  0x6ffffffb
#define DT_VERDEF   0x6ffffffc
#define DT_VERDEFNUM    0x6ffffffd
#define DT_VERNEED  0x6ffffffe
#define DT_VERNEEDNUM   0x6fffffff
#define OLD_DT_HIOS     0x6fffffff
#define DT_LOPROC   0x70000000
#define DT_HIPROC   0x7fffffff

/* This info is needed when parsing the symbol table */
#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2

#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_COMMON  5
#define STT_TLS     6

#define ELF_ST_BIND(x)      ((x) >> 4)
#define ELF_ST_TYPE(x)      (((unsigned int) x) & 0xf)
#define ELF32_ST_BIND(x)    ELF_ST_BIND(x)
#define ELF32_ST_TYPE(x)    ELF_ST_TYPE(x)
#define ELF64_ST_BIND(x)    ELF_ST_BIND(x)
#define ELF64_ST_TYPE(x)    ELF_ST_TYPE(x)

typedef struct {
    Elf32_Sword d_tag;
    union{
        Elf32_Sword d_val;
        Elf32_Addr  d_ptr;
    } d_un;
} Elf32_Dyn;

/* The following are used with relocations */
#define ELF32_R_ADDR_BASE(i)    (((i) >> 16) & 0xFF)
#define ELF32_R_OFS_BASE(i)     (((i) >> 8) & 0xFF)
#define ELF32_R_TYPE(i)         (i & 0xFF)

#define R_MIPS_NONE     0
#define R_MIPS_16       1
#define R_MIPS_32       2
#define R_MIPS_REL32    3
#define R_MIPS_26       4
#define R_MIPS_HI16     5
#define R_MIPS_LO16     6
#define R_MIPS_GPREL16  7
#define R_MIPS_LITERAL  8
#define R_MIPS_GOT16    9
#define R_MIPS_PC16     10
#define R_MIPS_CALL16   11
#define R_MIPS_GPREL32  12

typedef struct elf32_rel
{
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
} __attribute__ ((packed)) Elf32_Rel;

typedef struct elf32_rela
{
    Elf32_Addr  r_offset;
    Elf32_Word  r_info;
    Elf32_Sword r_addend;
} Elf32_Rela;

typedef struct elf32_sym
{
    Elf32_Word  st_name;
    Elf32_Addr  st_value;
    Elf32_Word  st_size;
    unsigned char   st_info;
    unsigned char   st_other;
    Elf32_Half  st_shndx;
} Elf32_Sym;

#define EI_NIDENT   16

typedef struct elf32_hdr
{
    unsigned char   e_ident[EI_NIDENT];
    Elf32_Half      e_type;
    Elf32_Half      e_machine;
    Elf32_Word      e_version;
    Elf32_Addr      e_entry;  /* Entry point */
    Elf32_Off       e_phoff;
    Elf32_Off       e_shoff;
    Elf32_Word      e_flags;
    Elf32_Half      e_ehsize;
    Elf32_Half      e_phentsize;
    Elf32_Half      e_phnum;
    Elf32_Half      e_shentsize;
    Elf32_Half      e_shnum;
    Elf32_Half      e_shstrndx;
} __attribute__ ((packed)) Elf32_Ehdr;

/* These constants define the permissions on sections in the program
   header, p_flags. */
#define PF_R        0x4
#define PF_W        0x2
#define PF_X        0x1

typedef struct elf32_phdr
{
    Elf32_Word  p_type;
    Elf32_Off   p_offset;
    Elf32_Addr  p_vaddr;
    Elf32_Addr  p_paddr;
    Elf32_Word  p_filesz;
    Elf32_Word  p_memsz;
    Elf32_Word  p_flags;
    Elf32_Word  p_align;
} __attribute__ ((packed)) Elf32_Phdr;

/* sh_type */
#define SHT_NULL    0
#define SHT_PROGBITS    1
#define SHT_SYMTAB  2
#define SHT_STRTAB  3
#define SHT_RELA    4
#define SHT_HASH    5
#define SHT_DYNAMIC 6
#define SHT_NOTE    7
#define SHT_NOBITS  8
#define SHT_REL     9
#define SHT_SHLIB   10
#define SHT_DYNSYM  11
#define SHT_NUM     12
#define SHT_LOPROC  0x70000000
#define SHT_HIPROC  0x7fffffff
#define SHT_LOUSER  0x80000000
#define SHT_HIUSER  0xffffffff

/* sh_flags */
#define SHF_WRITE       0x1
#define SHF_ALLOC       0x2
#define SHF_EXECINSTR       0x4
#define SHF_RELA_LIVEPATCH  0x00100000
#define SHF_RO_AFTER_INIT   0x00200000
#define SHF_MASKPROC        0xf0000000

/* special section indexes */
#define SHN_UNDEF   0
#define SHN_LORESERVE   0xff00
#define SHN_LOPROC  0xff00
#define SHN_HIPROC  0xff1f
#define SHN_LIVEPATCH   0xff20
#define SHN_ABS     0xfff1
#define SHN_COMMON  0xfff2
#define SHN_HIRESERVE   0xffff

typedef struct elf32_shdr {
    Elf32_Word  sh_name;
    Elf32_Word  sh_type;
    Elf32_Word  sh_flags;
    Elf32_Addr  sh_addr;
    Elf32_Off   sh_offset;
    Elf32_Word  sh_size;
    Elf32_Word  sh_link;
    Elf32_Word  sh_info;
    Elf32_Word  sh_addralign;
    Elf32_Word  sh_entsize;
} __attribute__ ((packed)) Elf32_Shdr;

#define EI_MAG0     0       /* e_ident[] indexes */
#define EI_MAG1     1
#define EI_MAG2     2
#define EI_MAG3     3
#define EI_CLASS    4
#define EI_DATA     5
#define EI_VERSION  6
#define EI_OSABI    7
#define EI_PAD      8

#define ELFMAG0     0x7f        /* EI_MAG */
#define ELFMAG1     'E'
#define ELFMAG2     'L'
#define ELFMAG3     'F'
#define ELFMAG      "\177ELF"
#define SELFMAG     4

#define ELFCLASSNONE    0       /* EI_CLASS */
#define ELFCLASS32  1
#define ELFCLASS64  2
#define ELFCLASSNUM 3

#define ELFDATANONE 0       /* e_ident[EI_DATA] */
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define EV_NONE     0       /* e_version, EI_VERSION */
#define EV_CURRENT  1
#define EV_NUM      2

#define ELFOSABI_NONE   0
#define ELFOSABI_LINUX  3

#ifndef ELF_OSABI
#define ELF_OSABI ELFOSABI_NONE
#endif

/* Note header in a PT_NOTE section */
typedef struct elf32_note
{
    Elf32_Word  n_namesz;   /* Name size */
    Elf32_Word  n_descsz;   /* Content size */
    Elf32_Word  n_type;     /* Content type */
} Elf32_Nhdr;

#endif
