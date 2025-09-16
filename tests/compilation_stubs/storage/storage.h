#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define EXT_PATH(path) path

#define FSE_OK 0
#define FSAM_READ 0
#define FSAM_WRITE 1
#define FSOM_OPEN_EXISTING 0
#define FSOM_CREATE_ALWAYS 1

typedef struct Storage Storage;
typedef struct File File;

typedef struct {
  uint64_t size;
} FileInfo;

#ifdef __cplusplus
extern "C" {
#endif

bool storage_file_exists(Storage *storage, const char *path);
File *storage_file_alloc(Storage *storage);
void storage_file_free(File *file);
bool storage_file_open(File *file, const char *path, int access_mode,
                       int open_mode);
void storage_file_close(File *file);
uint16_t storage_file_read(File *file, void *buffer, uint16_t length);
uint16_t storage_file_write(File *file, const void *buffer, uint16_t length);
int storage_common_stat(Storage *storage, const char *path, FileInfo *info);
int storage_simply_mkdir(Storage *storage, const char *path);

#ifdef __cplusplus
}
#endif
