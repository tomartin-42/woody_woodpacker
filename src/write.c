#include "../includes/woody.h"
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>

static int write_all(int fd, const void *buffer, size_t size) {
  const unsigned char *cursor;
  ssize_t written;

  cursor = buffer;
  while (size > 0) {
    written = write(fd, cursor, size);
    if (written < 0) {
      if (errno == EINTR)
        continue;
      return (0);
    }
    if (written == 0)
      return (0);
    cursor += (size_t)written;
    size -= (size_t)written;
  }
  return (1);
}

int write_woody(unsigned char *origin_file, size_t origin_len,
                t_cave_info *cave_info) {
  int fd;

  // Crea el ejecutable de salida o elimina su contenido anterior.
  if ((fd = open("woody", O_WRONLY | O_CREAT | O_TRUNC, 0755)) == -1) {
    return (0);
  }

  // Escribe primero el ELF original, ya cifrado y con su cabecera parcheada.
  // El seek deja el padding necesario hasta la nueva tabla PHDR.
  if (!write_all(fd, origin_file, origin_len) ||
      lseek(fd, (off_t)cave_info->phdr_offset, SEEK_SET) == -1) {
    close(fd);
    return (0);
  }

  // Añade la tabla PHDR ampliada y avanza hasta el comienzo del payload.
  if (!write_all(fd, cave_info->new_phdrs, cave_info->phdr_size) ||
      lseek(fd, (off_t)cave_info->payload_offset, SEEK_SET) == -1) {
    close(fd);
    return (0);
  }

  // Escribe el payload que descifrará .text antes de saltar al entry original.
  if (!write_all(fd, cave_info->payload, cave_info->payload_size)) {
    close(fd);
    return (0);
  }

  close(fd);
  return (1);
}
