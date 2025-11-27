#pragma once

#include <stdarg.h>
#include <stdint.h>

#ifndef SLOG_MSG_MAX
#define SLOG_MSG_MAX 256
#endif

#ifndef SLOG_LINE_MAX
#define SLOG_LINE_MAX 320
#endif

typedef enum {
  SLOG_VERBOSE = 0,
  SLOG_DEBUG,
  SLOG_INFO,
  SLOG_WARN,
  SLOG_ERROR,
} slog_level_t;

typedef uint32_t (*slog_timestamp_fn_t)(void);

typedef void (*slog_backend_fn_t)(slog_level_t level, const char *tag, const char *line);

void slog_set_timestamp_cb(slog_timestamp_fn_t func);
void slog_set_backend_cb(slog_backend_fn_t func);

void slog_v(slog_level_t level, const char *tag, const char *fmt, va_list args);

void slog(slog_level_t level, const char *tag, const char *fmt, ...);

#define SLOGV(fmt, ...) slog(SLOG_VERBOSE, TAG, fmt, ##__VA_ARGS__)
#define SLOGD(fmt, ...) slog(SLOG_DEBUG, TAG, fmt, ##__VA_ARGS__)
#define SLOGI(fmt, ...) slog(SLOG_INFO, TAG, fmt, ##__VA_ARGS__)
#define SLOGW(fmt, ...) slog(SLOG_WARN, TAG, fmt, ##__VA_ARGS__)
#define SLOGE(fmt, ...) slog(SLOG_ERROR, TAG, fmt, ##__VA_ARGS__)
