#include "slog/platform/esp.h"
#include "slog.h"
#include <esp_log_level.h>
#include <esp_log_timestamp.h>
#include <esp_log_write.h>
#include <stdint.h>

static uint32_t esp_ts_ms() {
  return (uint32_t) esp_log_timestamp();
}

static esp_log_level_t map_level(slog_level_t level) {
  switch (level) {
  case SLOG_VERBOSE:
    return ESP_LOG_VERBOSE;
  case SLOG_INFO:
    return ESP_LOG_INFO;
  case SLOG_WARN:
    return ESP_LOG_WARN;
  case SLOG_ERROR:
    return ESP_LOG_ERROR;
  case SLOG_DEBUG:
  default:
    return ESP_LOG_DEBUG;
  }
}

static void esp_backend(slog_level_t level, const char *tag, const char *line) {
  esp_log_level_t esp_level = map_level(level);
  esp_log_write(esp_level, tag, "%s\n", line);
}

void slog_esp_init() {
  slog_set_timestamp_cb(esp_log_timestamp);
  slog_set_backend_cb(esp_backend);
}
