#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct SceneManager SceneManager;

typedef struct {
  uint8_t type;
} SceneManagerEvent;

typedef struct {
  void (**on_enter_handlers)(void *context);
  bool (**on_event_handlers)(void *context, SceneManagerEvent event);
  void (**on_exit_handlers)(void *context);
  size_t scene_num;
} SceneManagerHandlers;

typedef enum {
  SceneManagerEventTypeCustom = 0,
  SceneManagerEventTypeTick = 1,
} SceneManagerEventType;
