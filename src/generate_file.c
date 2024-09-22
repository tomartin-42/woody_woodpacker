#include "../includes/woody.h"
#include <elf.h>
#include <sys/types.h>

void put_file(t_woody *woody, void *origin_file, ssize_t origin_len) {
  int fd;
  fd = open("woody", O_WRONLY | O_CREAT, 0777);
  write(fd, woody->file, woody->file_size);
  // if (write(fd, woody->file, woody->file_size) != (long int)woody->file_size)
  // {
  //   launch_error(WRITE_FAIL, origin_file, origin_len);
  // }
  close(fd);
  clean_up(woody, origin_file, origin_len);
}

void put_file_32(t_woody_32 *woody, void *origin_file, ssize_t origin_len) {
  int fd;
  (void)origin_len;
  (void)origin_file;
  fd = open("woody", O_WRONLY | O_CREAT, 0777);
  write(fd, woody->file, woody->file_size);
  // if (write(fd, woody->file, woody->file_size) != (long int)woody->file_size)
  // {
  //   launch_error(WRITE_FAIL, origin_file, origin_len);
  // }
  clean_up_32(woody, origin_file, origin_len);
  close(fd);
}
