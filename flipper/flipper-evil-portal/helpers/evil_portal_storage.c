#include "evil_portal_storage.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

static Storage *evil_portal_open_storage() {
  return furi_record_open(RECORD_STORAGE);
}

static void evil_portal_close_storage() { furi_record_close(RECORD_STORAGE); }

static uint8_t *evil_portal_strdup_safe(const char *text) {
  if (!text) {
    text = "";
  }

  size_t len = strlen(text);
  char *copy = malloc(len + 1);
  if (copy) {
    memcpy(copy, text, len + 1);
  }

  return (uint8_t *)copy;
}

void evil_portal_read_index_html(void *context) {

  Evil_PortalApp *app = context;
  Storage *storage = evil_portal_open_storage();
  FileInfo fi;

  if (app->index_html) {
    free(app->index_html);
    app->index_html = NULL;
  }

  bool loaded = false;

  if (storage_common_stat(storage, EVIL_PORTAL_INDEX_SAVE_PATH, &fi) == FSE_OK &&
      fi.size > 0) {
    File *index_html = storage_file_alloc(storage);
    if (index_html) {
      bool file_opened =
          storage_file_open(index_html, EVIL_PORTAL_INDEX_SAVE_PATH, FSAM_READ,
                            FSOM_OPEN_EXISTING);
      if (file_opened) {
        size_t buffer_size = (size_t)fi.size;
        uint8_t *buffer = malloc(buffer_size + 1);
        if (buffer) {
          size_t read = 0;
          while (read < buffer_size) {
            size_t to_read = buffer_size - read;
            if (to_read > UINT16_MAX) {
              to_read = UINT16_MAX;
            }
            uint16_t now_read =
                storage_file_read(index_html, buffer + read, (uint16_t)to_read);
            if (now_read == 0) {
              break;
            }
            read += now_read;
          }
          buffer[read] = '\0';
          if (read > 0) {
            app->index_html = buffer;
            loaded = true;
          } else {
            free(buffer);
          }
        }
        storage_file_close(index_html);
      }
      storage_file_free(index_html);
    }
  }

  if (!loaded) {
    const char *html_error =
        "<b>Evil portal</b><br>Unable to read the html file.<br>"
        "Is the SD Card set up correctly? <br>See instructions @ "
        "github.com/bigbrodude6119/flipper-zero-evil-portal<br>"
        "Under the 'Install pre-built app on the flipper' section.";
    app->index_html = evil_portal_strdup_safe(html_error);
  }

  evil_portal_close_storage();
}

void evil_portal_read_failed_html(void *context) {

  Evil_PortalApp *app = context;
  Storage *storage = evil_portal_open_storage();
  FileInfo fi;

  if (app->failed_html) {
    free(app->failed_html);
    app->failed_html = NULL;
  }
  app->has_failed_html = false;

  if (storage_common_stat(storage, EVIL_PORTAL_FAILED_SAVE_PATH, &fi) == FSE_OK &&
      fi.size > 0) {
    File *failed_html = storage_file_alloc(storage);
    if (failed_html) {
      bool file_opened = storage_file_open(failed_html, EVIL_PORTAL_FAILED_SAVE_PATH,
                                           FSAM_READ, FSOM_OPEN_EXISTING);
      if (file_opened) {
        size_t buffer_size = (size_t)fi.size;
        app->failed_html = malloc(buffer_size + 1);
        if (app->failed_html) {
          size_t read = 0;
          while (read < buffer_size) {
            size_t to_read = buffer_size - read;
            if (to_read > UINT16_MAX)
              to_read = UINT16_MAX;
            uint16_t now_read = storage_file_read(
                failed_html, app->failed_html + read, (uint16_t)to_read);
            if (now_read == 0)
              break;
            read += now_read;
          }
          app->failed_html[read] = '\0';
          app->has_failed_html = (read > 0);
          if (!app->has_failed_html) {
            free(app->failed_html);
            app->failed_html = NULL;
          }
        }
      }
      if (file_opened) {
        storage_file_close(failed_html);
      }
      storage_file_free(failed_html);
    }
  }

  evil_portal_close_storage();
}

void evil_portal_read_ap_name(void *context) {
  Evil_PortalApp *app = context;
  Storage *storage = evil_portal_open_storage();
  FileInfo fi;

  if (app->ap_name) {
    free(app->ap_name);
    app->ap_name = NULL;
  }

  bool loaded = false;

  if (storage_common_stat(storage, EVIL_PORTAL_AP_SAVE_PATH, &fi) == FSE_OK &&
      fi.size > 0) {
    File *ap_name = storage_file_alloc(storage);
    if (ap_name) {
      bool file_opened = storage_file_open(ap_name, EVIL_PORTAL_AP_SAVE_PATH,
                                           FSAM_READ, FSOM_OPEN_EXISTING);
      if (file_opened) {
        size_t buffer_size = (size_t)fi.size;
        uint8_t *buffer = malloc(buffer_size + 1);
        if (buffer) {
          size_t read = 0;
          while (read < buffer_size) {
            size_t to_read = buffer_size - read;
            if (to_read > UINT16_MAX) {
              to_read = UINT16_MAX;
            }
            uint16_t now_read =
                storage_file_read(ap_name, buffer + read, (uint16_t)to_read);
            if (now_read == 0) {
              break;
            }
            read += now_read;
          }
          buffer[read] = '\0';
          if (read > 0) {
            app->ap_name = buffer;
            loaded = true;
          } else {
            free(buffer);
          }
        }
        storage_file_close(ap_name);
      }
      storage_file_free(ap_name);
    }
  }

  if (!loaded) {
    app->ap_name = evil_portal_strdup_safe("Evil Portal");
  }
  evil_portal_close_storage();
}

char *sequential_file_resolve_path(Storage *storage, const char *dir,
                                   const char *prefix, const char *extension) {
  if (storage == NULL || dir == NULL || prefix == NULL || extension == NULL) {
    return NULL;
  }

  char file_path[256];
  int file_index = 0;

  do {
    if (snprintf(file_path, sizeof(file_path), "%s/%s_%d.%s", dir, prefix,
                 file_index, extension) < 0) {
      return NULL;
    }
    file_index++;
  } while (storage_file_exists(storage, file_path));

  size_t path_len = strlen(file_path);
  char *result = malloc(path_len + 1);
  if (result) {
    memcpy(result, file_path, path_len + 1);
  }
  return result;
}

void write_logs(FuriString *portal_logs) {
  Storage *storage = evil_portal_open_storage();

  if (!storage_file_exists(storage, EVIL_PORTAL_LOG_SAVE_PATH)) {
    storage_simply_mkdir(storage, EVIL_PORTAL_LOG_SAVE_PATH);
  }

  char *seq_file_path = sequential_file_resolve_path(
      storage, EVIL_PORTAL_LOG_SAVE_PATH, "log", "txt");

  if (seq_file_path) {
    File *file = storage_file_alloc(storage);
    if (file) {
      if (storage_file_open(file, seq_file_path, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        storage_file_write(file, furi_string_get_cstr(portal_logs),
                           furi_string_utf8_length(portal_logs));
        storage_file_close(file);
      }
      storage_file_free(file);
    }
    free(seq_file_path);
  }
  evil_portal_close_storage();
}