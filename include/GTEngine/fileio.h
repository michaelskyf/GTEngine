#ifndef GTE_FILEIO_H
#define GTE_FILEIO_H
#include <aio.h>

char *file_read(const char *);
// Returns number of characters written
size_t file_write(const char *path, const char *content);
size_t file_append(const char *path, const char *content);

#endif
