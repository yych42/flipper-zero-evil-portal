#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FuriString FuriString;

#define RECORD_STORAGE "storage"

void *furi_record_open(const char *record_name);
void furi_record_close(const char *record_name);
size_t furi_string_utf8_length(const FuriString *str);
const char *furi_string_get_cstr(const FuriString *str);

#ifdef __cplusplus
}
#endif
