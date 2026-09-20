#include "../includes/woody.h"
#include "libft.h"
#include <elf.h>
#include <stddef.h>

// Comprueba que un rango está dentro del archivo
static int range_is_valid_64(size_t file_size, uint64_t offset, uint64_t size) {
  if (offset > file_size)
    return (0);
  if (size > file_size - offset)
    return (0);
  return (1);
}

static int validate_pt_load64(t_elf_info *elf_info) {
  const Elf64_Ehdr *ehdr;
  const Elf64_Phdr *phdrs;
  const Elf64_Phdr *text_segment;

  // Obtiene la cabecera ELF y su tabla de Program Headers
  ehdr = (const Elf64_Ehdr *)elf_info->ehdr;
  phdrs = (const Elf64_Phdr *)elf_info->phdrs;
  text_segment = NULL;

  for (size_t i = 0; i < ehdr->e_phnum; i++) {
    uint64_t file_delta;
    uint64_t virtual_delta;

    // Busca únicamente segmentos cargables y ejecutables
    if (phdrs[i].p_type != PT_LOAD || !(phdrs[i].p_flags & PF_X)) {
      continue;
    }

    // .text no puede comenzar antes que el segmento
    if (elf_info->text_offset < phdrs[i].p_offset ||
        elf_info->text_vaddr < phdrs[i].p_vaddr) {
      continue;
    }

    // Calcula la posición de .text dentro del segmento
    file_delta = elf_info->text_offset - phdrs[i].p_offset;
    virtual_delta = elf_info->text_vaddr - phdrs[i].p_vaddr;

    // Comprueba que .text quepa en la parte del segmento presente en el archivo
    if (file_delta > phdrs[i].p_filesz ||
        elf_info->text_size > phdrs[i].p_filesz - file_delta) {
      continue;
    }

    // Comprueba que .text quepa también en el rango virtual del segmento
    if (virtual_delta > phdrs[i].p_memsz ||
        elf_info->text_size > phdrs[i].p_memsz - virtual_delta) {
      continue;
    }

    // El desplazamiento físico y virtual de .text debe coincidir
    if (file_delta != virtual_delta) {
      continue;
    }

    // Solo un segmento puede contener .text
    if (text_segment != NULL)
      return (0);

    text_segment = &phdrs[i];
  }

  // No existe un PT_LOAD ejecutable que contenga .text
  if (text_segment == NULL)
    return (0);

  // Guarda el segmento que contiene la sección .text
  elf_info->text_segment = (void *)text_segment;
  return (1);
}

static int validate_pt_load32(t_elf_info *elf_info) {
  const Elf32_Ehdr *ehdr;
  const Elf32_Phdr *phdrs;
  const Elf32_Phdr *text_segment;

  // Obtiene la cabecera ELF y su tabla de Program Headers
  ehdr = (const Elf32_Ehdr *)elf_info->ehdr;
  phdrs = (const Elf32_Phdr *)elf_info->phdrs;
  text_segment = NULL;

  for (size_t i = 0; i < ehdr->e_phnum; i++) {
    uint64_t file_delta;
    uint64_t virtual_delta;

    // Busca únicamente segmentos cargables y ejecutables
    if (phdrs[i].p_type != PT_LOAD || !(phdrs[i].p_flags & PF_X))
      continue;

    // .text no puede comenzar antes que el segmento
    if (elf_info->text_offset < phdrs[i].p_offset ||
        elf_info->text_vaddr < phdrs[i].p_vaddr) {
      continue;
    }

    // Calcula la posición de .text dentro del segmento
    file_delta = elf_info->text_offset - phdrs[i].p_offset;
    virtual_delta = elf_info->text_vaddr - phdrs[i].p_vaddr;

    // Comprueba que .text quepa en la parte del segmento presente en el archivo
    if (file_delta > phdrs[i].p_filesz ||
        elf_info->text_size > phdrs[i].p_filesz - file_delta) {
      continue;
    }

    // Comprueba que .text quepa también en el rango virtual del segmento
    if (virtual_delta > phdrs[i].p_memsz ||
        elf_info->text_size > phdrs[i].p_memsz - virtual_delta) {
      continue;
    }

    // El desplazamiento físico y virtual de .text debe coincidir
    if (file_delta != virtual_delta)
      continue;

    // Solo un segmento puede contener .text
    if (text_segment != NULL)
      return (0);

    text_segment = &phdrs[i];
  }

  // No existe un PT_LOAD ejecutable que contenga .text
  if (text_segment == NULL)
    return (0);

  // Guarda el segmento que contiene la sección .text
  elf_info->text_segment = (void *)text_segment;
  return (1);
}

static int validate_text64(const unsigned char *original_file, size_t file_size,
                           t_elf_info *elf_info) {

  const Elf64_Ehdr *ehdr;
  const Elf64_Shdr *shdrs;
  const Elf64_Shdr *shstr_shdr;
  const Elf64_Shdr *text;
  const char *section_names;

  // Obtiene las cabeceras de sección y su tabla de nombres
  ehdr = (const Elf64_Ehdr *)original_file;
  shdrs = (const Elf64_Shdr *)(original_file + ehdr->e_shoff);
  shstr_shdr = &shdrs[ehdr->e_shstrndx];
  section_names = (const char *)(original_file + shstr_shdr->sh_offset);

  text = NULL;
  // Recorre las secciones para localizar un único nombre .text válido
  for (size_t i = 0; i < ehdr->e_shnum; i++) {
    size_t remain;

    if (shdrs[i].sh_name >= shstr_shdr->sh_size)
      return (0);
    remain = shstr_shdr->sh_size - shdrs[i].sh_name;
    // La comparación incluye el terminador para exigir el nombre exacto
    if (remain >= 6 &&
        ft_strncmp(section_names + shdrs[i].sh_name, ".text", 6) == 0) {
      // Solo admite una sección .text
      if (text != NULL)
        return (0);
      text = &shdrs[i];
    }
  }
  // No hay sección .text
  if (text == NULL) {
    return (0);
  }
  // Comprueba las propiedades de la sección .text
  if (text->sh_type != SHT_PROGBITS || !(text->sh_flags & SHF_ALLOC) ||
      !(text->sh_flags & SHF_EXECINSTR) || text->sh_size == 0) {
    return (0);
  }

  // Comprueba que el contenido de .text esté dentro del archivo
  if (!range_is_valid_64(file_size, text->sh_offset, text->sh_size))
    return (0);

  // Guarda la información normalizada de .text
  elf_info->text_shdr = (void *)text;
  elf_info->text_offset = text->sh_offset;
  elf_info->text_vaddr = text->sh_addr;
  elf_info->text_size = text->sh_size;

  return (1);
}

static int validate_text32(const unsigned char *original_file, size_t file_size,
                           t_elf_info *elf_info) {
  const Elf32_Ehdr *ehdr;
  const Elf32_Shdr *shdrs;
  const Elf32_Shdr *shstr_shdr;
  const Elf32_Shdr *text;
  const char *section_names;

  // Obtiene las cabeceras de sección y su tabla de nombres
  ehdr = (const Elf32_Ehdr *)original_file;
  shdrs = (const Elf32_Shdr *)(original_file + ehdr->e_shoff);
  shstr_shdr = &shdrs[ehdr->e_shstrndx];
  section_names = (const char *)(original_file + shstr_shdr->sh_offset);

  text = NULL;
  // Recorre las secciones para localizar un único nombre .text válido
  for (size_t i = 0; i < ehdr->e_shnum; i++) {
    size_t remain;

    if (shdrs[i].sh_name >= shstr_shdr->sh_size)
      return (0);
    remain = shstr_shdr->sh_size - shdrs[i].sh_name;
    // La comparación incluye el terminador para exigir el nombre exacto
    if (remain >= 6 &&
        ft_strncmp(section_names + shdrs[i].sh_name, ".text", 6) == 0) {
      // Solo admite una sección .text
      if (text != NULL)
        return (0);
      text = &shdrs[i];
    }
  }
  // No hay sección .text
  if (text == NULL)
    return (0);

  // Comprueba las propiedades de la sección .text
  if (text->sh_type != SHT_PROGBITS || !(text->sh_flags & SHF_ALLOC) ||
      !(text->sh_flags & SHF_EXECINSTR) || text->sh_size == 0) {
    return (0);
  }

  // Comprueba que el contenido de .text esté dentro del archivo
  if (!range_is_valid_64(file_size, text->sh_offset, text->sh_size))
    return (0);

  // Guarda la información normalizada de .text
  elf_info->text_shdr = (void *)text;
  elf_info->text_offset = text->sh_offset;
  elf_info->text_vaddr = text->sh_addr;
  elf_info->text_size = text->sh_size;

  return (1);
}

// Validaciones y finder segmento .text
int validate_text_segment(const unsigned char *original_file, size_t file_size,
                           t_elf_info *elf_info) {
  if (original_file == NULL || elf_info == NULL)
    return (0);
  if (elf_info->elf_class == WOODY_ELF64) {
    if (!validate_text64(original_file, file_size, elf_info) ||
        !validate_pt_load64(elf_info)) {
      return (0);
    }
    return (1);
  }
  if (elf_info->elf_class == WOODY_ELF32) {
    if (!validate_text32(original_file, file_size, elf_info) ||
        !validate_pt_load32(elf_info)) {
      return (0);
    }
    return (1);
  }
  return (0);
}

static int validate_phdr64(const unsigned char *original_file,
                           const Elf64_Ehdr *ehdr, uint64_t *max_load_end) {
  const Elf64_Phdr *phdrs;
  int pt_load_exits = 0;

  *max_load_end = 0;

  phdrs = (const Elf64_Phdr *)(original_file + ehdr->e_phoff);
  for (size_t i = 0; i < ehdr->e_phnum; i++) {
    const Elf64_Phdr *phdr = &phdrs[i];

    // Comprobaciones para segmentos de tipo PT_LOAD
    if (phdr->p_type == PT_LOAD) {
      pt_load_exits++;

      // Tamaño en memoria debe de ser = o > que en fichero
      if (phdr->p_filesz > phdr->p_memsz)
        return (0);

      // Tiene una virtual addres correcta y dentro de rango
      if (phdr->p_vaddr > UINT64_MAX - phdr->p_memsz)
        return (0);

      // Conserva el final virtual más alto de los segmentos cargables
      if (phdr->p_vaddr + phdr->p_memsz > *max_load_end)
        *max_load_end = phdr->p_vaddr + phdr->p_memsz;
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
  return (1);
}

static int validate_phdr32(const unsigned char *original_file,
                           const Elf32_Ehdr *ehdr, uint64_t *max_load_end) {
  const Elf32_Phdr *phdrs;
  int pt_load_exits = 0;

  *max_load_end = 0;
  phdrs = (const Elf32_Phdr *)(original_file + ehdr->e_phoff);
  for (size_t i = 0; i < ehdr->e_phnum; i++) {
    const Elf32_Phdr *phdr = &phdrs[i];

    // Comprobaciones para segmentos de tipo PT_LOAD
    if (phdr->p_type == PT_LOAD) {
      pt_load_exits++;

      // Tamaño en memoria debe de ser = o > que en fichero
      if (phdr->p_filesz > phdr->p_memsz)
        return (0);

      // Comprueba que el final virtual se pueda representar en ELF32
      if (phdr->p_vaddr > UINT32_MAX - phdr->p_memsz)
        return (0);

      // Conserva el final virtual más alto de los segmentos cargables
      if ((uint64_t)phdr->p_vaddr + phdr->p_memsz > *max_load_end)
        *max_load_end = (uint64_t)phdr->p_vaddr + phdr->p_memsz;
    }
  }
  // Debe de haber como mínimo un PT_LOAD
  if (pt_load_exits == 0) {
    return (0);
  }
  return (1);
}

static int validate_shdr32(const unsigned char *original_file,
                           size_t original_len, const Elf32_Ehdr *ehdr) {
  const Elf32_Shdr *shdrs;
  const Elf32_Shdr *shstr_shdr;

  // Comprobación que la stringtab es de tipo SHT_STRTAB
  shdrs = (const Elf32_Shdr *)(original_file + ehdr->e_shoff);
  shstr_shdr = &shdrs[ehdr->e_shstrndx];
  if (shstr_shdr->sh_type != SHT_STRTAB)
    return (0);

  // Comprueba que la sección de nombres esté dentro del archivo
  if (!range_is_valid_64(original_len, shstr_shdr->sh_offset,
                         shstr_shdr->sh_size)) {
    return (0);
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

  return (1);
}

static int validate_elf64(const unsigned char *original_file,
                          size_t original_len, t_elf_info *elf_info) {
  const Elf64_Ehdr *ehdr;
  const Elf64_Phdr *phdrs;
  const Elf64_Shdr *shdrs;
  t_elf_info result = {0};
  uint64_t max_load_end;

  // Protección para no chequear un archivo que no tiene tamaño suficiente
  // como para albergar Elf64_Ehdr
  if (original_file == NULL || original_len < sizeof(Elf64_Ehdr)) {
    return (0);
  }
  ehdr = (const Elf64_Ehdr *)original_file;
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
  // Si existen Program Headers, su tabla debe tener un offset válido
  if (ehdr->e_phoff == 0) {
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
  // Debe quedar espacio en e_phnum para añadir un nuevo Program Header
  if (ehdr->e_phnum >= PN_XNUM - 1) {
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

  if (!validate_phdr64(original_file, ehdr, &max_load_end)) {
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

  phdrs = (const Elf64_Phdr *)(original_file + ehdr->e_phoff);
  shdrs = (const Elf64_Shdr *)(original_file + ehdr->e_shoff);
  result.elf_class = WOODY_ELF64;
  result.ehdr = (void *)ehdr;
  result.phdrs = (void *)phdrs;
  result.shdrs = (void *)shdrs;
  result.old_entry = ehdr->e_entry;
  result.max_load_end = max_load_end;
  *elf_info = result;

  return (1);
}

static int validate_elf32(const unsigned char *original_file,
                          size_t original_len, t_elf_info *elf_info) {
  const Elf32_Ehdr *ehdr;
  const Elf32_Phdr *phdrs;
  const Elf32_Shdr *shdrs;
  t_elf_info result = {0};
  uint64_t max_load_end;

  // Protección para no leer un archivo menor que una cabecera ELF32
  if (original_file == NULL || original_len < sizeof(Elf32_Ehdr)) {
    return (0);
  }
  ehdr = (const Elf32_Ehdr *)original_file;

  // Comprueba que el archivo sea para arquitectura Intel x86 de 32 bits
  if (ehdr->e_machine != EM_386) {
    return (0);
  }
  // Comprueba que sea un ejecutable ET_EXEC o ET_DYN, formato usado por PIE
  if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN) {
    return (0);
  }
  if (ehdr->e_version != EV_CURRENT || ehdr->e_ehsize != sizeof(Elf32_Ehdr)) {
    return (0);
  }

  // Comprueba la presencia y el formato de la tabla de Program Headers
  if (ehdr->e_phnum == 0 || ehdr->e_phoff == 0 ||
      ehdr->e_phentsize != sizeof(Elf32_Phdr)) {
    return (0);
  }
  // Rechaza formatos extendidos y reserva una entrada para el nuevo segmento
  if (ehdr->e_phnum == PN_XNUM || ehdr->e_phnum >= PN_XNUM - 1 ||
      ehdr->e_shstrndx == SHN_XINDEX) {
    return (0);
  }

  // Comprueba la presencia y el formato de la tabla de Section Headers
  if (ehdr->e_shnum == 0 || ehdr->e_shoff == 0 ||
      ehdr->e_shentsize != sizeof(Elf32_Shdr) ||
      ehdr->e_shstrndx == SHN_UNDEF || ehdr->e_shstrndx >= ehdr->e_shnum) {
    return (0);
  }

  // Comprueba que la tabla PHDR completa esté dentro del archivo
  if (ehdr->e_phoff > original_len ||
      ehdr->e_phnum > (original_len - ehdr->e_phoff) / sizeof(Elf32_Phdr)) {
    return (0);
  }
  if (!validate_phdr32(original_file, ehdr, &max_load_end)) {
    return (0);
  }

  // Comprueba que la tabla SHDR completa esté dentro del archivo
  if (ehdr->e_shoff > original_len ||
      ehdr->e_shnum > (original_len - ehdr->e_shoff) / sizeof(Elf32_Shdr)) {
    return (0);
  }
  if (!validate_shdr32(original_file, original_len, ehdr)) {
    return (0);
  }

  phdrs = (const Elf32_Phdr *)(original_file + ehdr->e_phoff);
  shdrs = (const Elf32_Shdr *)(original_file + ehdr->e_shoff);
  result.elf_class = WOODY_ELF32;
  result.ehdr = (void *)ehdr;
  result.phdrs = (void *)phdrs;
  result.shdrs = (void *)shdrs;
  result.old_entry = ehdr->e_entry;
  result.max_load_end = max_load_end;
  *elf_info = result;

  return (1);
}

int validate_elf(const unsigned char *original_file, size_t original_len,
                 t_elf_info *elf_info) {
  if (elf_info == NULL || !validate_ident(original_file, original_len)) {
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
