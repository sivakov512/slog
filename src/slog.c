#include "slog.h"
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

// ANSI colors
#define COL_RESET "\x1b[0m"
#define COL_RED "\x1b[0;31m"
#define COL_YELLOW "\x1b[0;33m"
#define COL_GREEN "\x1b[0;32m"
#define COL_CYAN "\x1b[0;36m"

static slog_timestamp_fn_t g_ts_cb = NULL;
static slog_backend_fn_t g_be_cb = NULL;

// ---- Default callbacks ----

static uint32_t default_ts(void) {
  return 0;
}

static void default_backend(slog_level_t level, const char *tag, const char *line) {
  (void) level;
  (void) tag;
  (void) line;
}

// ---- Configuration API ----

void slog_set_timestamp_cb(slog_timestamp_fn_t func) {
  g_ts_cb = func;
}

void slog_set_backend_cb(slog_backend_fn_t func) {
  g_be_cb = func;
}

// ---- Formatter ----

static const char *level_char(slog_level_t level) {
  switch (level) {
  case SLOG_VERBOSE:
    return "V";
  case SLOG_INFO:
    return "I";
  case SLOG_WARN:
    return "W";
  case SLOG_ERROR:
    return "E";
  case SLOG_DEBUG:
  default:
    return "D";
  }
}

static const char *level_color(slog_level_t level) {
  switch (level) {
  case SLOG_INFO:
    return COL_GREEN;
  case SLOG_WARN:
    return COL_YELLOW;
  case SLOG_ERROR:
    return COL_RED;
  case SLOG_VERBOSE:
  case SLOG_DEBUG:
  default:
    return COL_CYAN;
  }
}

static void format_line(char *out, size_t out_sz, slog_level_t level, const char *tag,
                        uint32_t ts_ms, const char *msg) {
  if (!tag) {
    tag = "";
  }

  // example: "I (123) host_ctrl: message"
  snprintf(out, out_sz, "%s%s (%" PRIu32 ") %s: %s%s", level_color(level), level_char(level), ts_ms,
           tag, msg, COL_RESET);
}

// ---- Logging functions ----

void slog_v(slog_level_t level, const char *tag, const char *fmt, va_list args) {
  char msg[SLOG_MSG_MAX];
  char line[SLOG_LINE_MAX];

  vsnprintf(msg, sizeof(msg), fmt, args);
  msg[sizeof(msg) - 1] = '\0';

  uint32_t ts = g_ts_cb ? g_ts_cb() : default_ts(); // NOLINT(readability-identifier-length)
  format_line(line, sizeof(line), level, tag, ts, msg);

  slog_backend_fn_t backend = g_be_cb ? g_be_cb : default_backend;
  backend(level, tag, line);
}

void slog(slog_level_t level, const char *tag, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  slog_v(level, tag, fmt, args);
  va_end(args);
}
