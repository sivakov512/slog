#include "slog/platform/generic.h"
#include "slog.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>

static uint32_t generic_timestamp() {
  struct timespec tspec;
  if (clock_gettime(CLOCK_MONOTONIC, &tspec) == 0) {
    return (uint32_t) ((tspec.tv_sec * 1000) +     // NOLINT(readability-magic-numbers)
                       (tspec.tv_nsec / 1000000)); // NOLINT(readability-magic-numbers)
  }
  return 0;
}

static void generic_backend(slog_level_t level,
                            const char *tag, // NOLINT( bugprone-easily-swappable-parameters)
                            const char *line) {
  (void) level;
  (void) tag;
  printf("%s\n", line);
  fflush(stdout);
}

void slog_generic_init() {
  slog_set_timestamp_cb(generic_timestamp);
  slog_set_backend_cb(generic_backend);
}
