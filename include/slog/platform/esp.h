#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void slog_esp_init();

// Logging helpers
#define RETURN_ON_ERROR(expr, fmt, ...)                                                            \
  do {                                                                                             \
    esp_err_t err = (expr);                                                                        \
    if (err != ESP_OK) {                                                                           \
      SLOGE(fmt " Err: %s", ##__VA_ARGS__, esp_err_to_name(err));                                  \
      return err;                                                                                  \
    }                                                                                              \
  } while (0)

#ifdef __cplusplus
}
#endif
