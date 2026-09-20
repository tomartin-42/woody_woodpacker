#include "../includes/woody.h"
#include <elf.h>

// Helper para comprobar que el string del nombre de sección tiene '\0' al final
// y evitar que falle ft_strcmp()
static int has_null_terminator(const char *string, size_t available) {
  for (size_t i = 0; i < available; i++) {
    if (string[i] == '\0')
      return (1);
  }
  return (0);
}

// Comprueba que el segmento está dentro del archivo
static int range_is_valid_64(size_t file_size, uint64_t offset, uint64_t size) {
  if (offset > file_size)
    return (0);
  if (size > file_size - offset)
    return (0);
  return (1);
}

static int validate_phdr64(const unsigned char *original_file,
                           size_t original_len, const Elf64_Ehdr *ehdr) {
  const Elf64_Phdr *phdrs;
  int pt_load_exits = 0;

  phdrs = (const Elf64_Phdr *)(original_file + ehdr->e_phoff);
  for (size_t i = 0; i < ehdr->e_phnum; i++) {
    const Elf64_Phdr *phdr = &phdrs[i];

    // Ignora las entradas no utilizadas de la tabla
    if (phdr->p_type == PT_NULL)
      continue;

    // Comprueba que los datos descritos por la entrada estén dentro del archivo
    if (!range_is_valid_64(original_len, phdr->p_offset, phdr->p_filesz)) {
      return (0);
    }

    // Comprobaciones para segmentos de tipo PT_LOAD
    if (phdr->p_type == PT_LOAD) {
      pt_load_exits++;
      // Tamaño en memoria debe de ser = o > que en fichero
      if (phdr->p_filesz > phdr->p_memsz)
        return (0);

      // Tiene una virtual addres correcta y dentro de rango
      if (phdr->p_vaddr > UINT64_MAX - phdr->p_memsz)
        return (0);

      // Comprobación de alineación
      if (phdr->p_align > 1 &&
          ((phdr->p_align & (phdr->p_align - 1)) != 0 ||
           phdr->p_offset % phdr->p_align != phdr->p_vaddr % phdr->p_align)) {
        return (0);
      }
    }
  }
  // Debe de haber como mínimo un PT_LOAD
  if (pt_load_exits == 0) {
    return (0);
  }
  return (1);
}

static int validate_shdr64(const unsigned char *original_file,
                           size_t original_len, const Elf64_Ehdr *ehdr) {
  const Elf64_Shdr *shdrs;
  const Elf64_Shdr *shstr_shdr;

  // Comprobación que la stringtab es de tipo SHT_STRTAB
  shdrs = (const Elf64_Shdr *)(original_file + ehdr->e_shoff);
  shstr_shdr = &shdrs[ehdr->e_shstrndx];
  if (shstr_shdr->sh_type != SHT_STRTAB)
    return (0);

  // Compruebo el rango de la sección de nombres y obtengo la sección de nombres
  if (!range_is_valid_64(original_len, shstr_shdr->sh_offset,
                         shstr_shdr->sh_size)) {
    return (0);
  }
  const char *section_names =
      (const char *)(original_file + shstr_shdr->sh_offset);

  for (size_t i = 0; i < ehdr->e_shnum; i++) {
    const Elf64_Shdr *shdr = &shdrs[i];

    if (shdr->sh_type != SHT_NOBITS &&
        !range_is_valid_64(original_len, shdr->sh_offset, shdr->sh_size)) {
      return (0);
    }
    // Compruebo secciones cargadas en memoria overflow:
    if ((shdr->sh_flags & SHF_ALLOC) &&
        shdr->sh_addr > UINT64_MAX - shdr->sh_size) {
      return (0);
    }
    // Compruebo alineamiento correcto
    if (shdr->sh_addralign > 1 &&
        (shdr->sh_addralign & (shdr->sh_addralign - 1)) != 0) {
      return (0);
    }

    // Comprueba la alineación virtual de las secciones cargadas en memoria
    if ((shdr->sh_flags & SHF_ALLOC) && shdr->sh_addralign > 1 &&
        shdr->sh_addr % shdr->sh_addralign != 0) {
      return (0);
    }

    // Comprueba que el offset del nombre esté dentro de la tabla de strings
    if (shdr->sh_name >= shstr_shdr->sh_size) {
      return (0);
    }

    // Comprobación nombre de sección termina en '\0' y no provocar overflow con
    // ft_strcmp()
    if (!has_null_terminator(section_names + shdr->sh_name,
                             shstr_shdr->sh_size - shdr->sh_name)) {
      return (0);
    }
  }

  return (1);
}

static int validate_ident(const unsigned char *original_file,
                          size_t original_len) {
  // Comprueba que se pueda leer la identificación completa del ELF
  if (original_file == NULL || original_len < EI_NIDENT) {
    return (0);
  }

  // Comprueba los bytes mágicos que identifican un archivo ELF
  if (original_file[EI_MAG0] != ELFMAG0 || original_file[EI_MAG1] != ELFMAG1 ||
      original_file[EI_MAG2] != ELFMAG2 || original_file[EI_MAG3] != ELFMAG3) {
    return (0);
  }

  // Comprueba que el ejecutable sea de 32 o 64 bits
  if (original_file[EI_CLASS] != ELFCLASS32 &&
      original_file[EI_CLASS] != ELFCLASS64) {
    return (0);
  }

  // Comprueba que use little-endian y una versión ELF válida
  if (original_file[EI_DATA] != ELFDATA2LSB ||
      original_file[EI_VERSION] != EV_CURRENT) {
    return (0);
  }

  // Los ejecutables Linux pueden declarar ABI System V o Linux
  if (original_file[EI_OSABI] != ELFOSABI_SYSV &&
      original_file[EI_OSABI] != ELFOSABI_LINUX) {
    return (0);
  }

  return (1);
}

static int validate_elf64(const unsigned char *original_file,
                          size_t original_len, t_elf_info *elf_info) {
  const Elf64_Ehdr *ehdr = (const Elf64_Ehdr *)original_file;

  // Protección para no chequear un archivo que no tiene tamaño suficiente
  // como para albergar Elf64_Ehdr
  if (original_file == NULL || original_len < sizeof(Elf64_Ehdr)) {
    return (0);
  }
  // Comprobación que el archivo es de arquitectura Intel 64
  if (ehdr->e_machine != EM_X86_64) {
    return (0);
  }
  // Comprueba que sea un ejecutable ET_EXEC o ET_DYN, formato usado por PIE
  if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN) {
    return (0);
  }
  // Comprueba la version del ELF
  // EV_CURRENT = versión válida
  if (ehdr->e_version != EV_CURRENT) {
    return (0);
  }
  // Comprueba que el tamaño del Elf64_Ehdr es consistente con lo indicado por
  // e_ehsize
  if (ehdr->e_ehsize != sizeof(Elf64_Ehdr)) {
    return (0);
  }
  // Comprueba que la tabla de Program Headers tenga entradas
  if (!(ehdr->e_phnum > 0)) {
    return (0);
  }
  // Comprueba la integridad del tamaño de la entradas a la tabla de Elf64_Phdr
  if (ehdr->e_phentsize != sizeof(Elf64_Phdr)) {
    return (0);
  }
  // Rechaza la numeración extendida de las tablas, todavía no soportada
  if (ehdr->e_phnum == PN_XNUM || ehdr->e_shstrndx == SHN_XINDEX) {
    return (0);
  }
  // Comprueba que haya Section Headers para poder localizar la sección .text
  if (ehdr->e_shnum == 0) {
    return (0);
  }
  // Comprueba que exista una tabla de Section Headers en el archivo
  if (ehdr->e_shoff == 0) {
    return (0);
  }
  // Comprueba que cada entrada tenga el tamaño de un Elf64_Shdr
  if (ehdr->e_shentsize != sizeof(Elf64_Shdr)) {
    return (0);
  }
  // Comprueba que exista la tabla de nombres de secciones y que su índice
  // pertenezca a la tabla de Section Headers
  if (ehdr->e_shstrndx == SHN_UNDEF || ehdr->e_shstrndx >= ehdr->e_shnum) {
    return (0);
  }

  // -------------------------------------------
  // Validación de los Program Headers
  // -------------------------------------------

  // Comprueba que la tabla esté dentro del archivo y que todas sus entradas
  // quepan en el buffer original
  if (ehdr->e_phoff > original_len ||
      ehdr->e_phnum > (original_len - ehdr->e_phoff) / sizeof(Elf64_Phdr)) {
    return (0);
  }

  if (!validate_phdr64(original_file, original_len, ehdr)) {
    return (0);
  }

  // -------------------------------------------
  // Validación de los Sections Headers
  // -------------------------------------------
  // Comprueba que la tabla esté dentro del archivo y que todas sus entradas
  // quepan en el buffer original
  if (ehdr->e_shoff > original_len ||
      ehdr->e_shnum > (original_len - ehdr->e_shoff) / sizeof(Elf64_Shdr)) {
    return (0);
  }

  if (!validate_shdr64(original_file, original_len, ehdr)) {
    return (0);
  }

  return (1);
}

static int validate_elf32(const unsigned char *original_file,
                          size_t original_len, t_elf_info *elf_info) {
  return (0);
}

int validate_elf(const unsigned char *original_file, size_t original_len,
                 t_elf_info *elf_info) {
  if (!validate_ident(original_file, original_len)) {
    return (0);
  }
  if (original_file[EI_CLASS] == ELFCLASS64) {
    if (!validate_elf64(original_file, original_len, elf_info)) {
      return (0);
    }
  } else {
    if (!validate_elf32(original_file, original_len, elf_info)) {
      return (0);
    }
  }
  return (1);
}
