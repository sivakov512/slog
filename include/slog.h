#pragma once

typedef enum {
    SLOG_VERBOSE = 0,
    SLOG_DEBUG,
    SLOG_INFO,
    SLOG_WARN,
    SLOG_ERROR,
} slog_level_t;

// ---- ESP-IDF ----
#if defined(ESP_PLATFORM) || defined(SLOG_BACKEND_ESP)

#include <esp_log.h>

// File scoped registration
#define SLOG_REGISTER(tag_literal)                                                  \
    static const char *const slog_tag__ = (tag_literal)
#define SLOG_TAG (slog_tag__)

// Direct dispatch, eg SLOGE("hello") -> ESP_LOGE(SLOG_TAG, "hello")
#define SLOGV(fmt, ...) ESP_LOGV(SLOG_TAG, fmt, ##__VA_ARGS__)
#define SLOGD(fmt, ...) ESP_LOGD(SLOG_TAG, fmt, ##__VA_ARGS__)
#define SLOGI(fmt, ...) ESP_LOGI(SLOG_TAG, fmt, ##__VA_ARGS__)
#define SLOGW(fmt, ...) ESP_LOGW(SLOG_TAG, fmt, ##__VA_ARGS__)
#define SLOGE(fmt, ...) ESP_LOGE(SLOG_TAG, fmt, ##__VA_ARGS__)

// Generic dispatch, eg SLOG(SLOG_ERROR, "hello") -> ESP_LOGE(SLOG_TAG, "hello")
#define SLOG(level, fmt, ...)                                                       \
    do {                                                                            \
        switch ((level)) {                                                          \
        case SLOG_ERROR:                                                            \
            ESP_LOGE(SLOG_TAG, fmt, ##__VA_ARGS__);                                 \
            break;                                                                  \
        case SLOG_WARN:                                                             \
            ESP_LOGW(SLOG_TAG, fmt, ##__VA_ARGS__);                                 \
            break;                                                                  \
        case SLOG_INFO:                                                             \
            ESP_LOGI(SLOG_TAG, fmt, ##__VA_ARGS__);                                 \
            break;                                                                  \
        case SLOG_VERBOSE:                                                          \
            ESP_LOGV(SLOG_TAG, fmt, ##__VA_ARGS__);                                 \
            break;                                                                  \
        case SLOG_DEBUG:                                                            \
        default:                                                                    \
            ESP_LOGD(SLOG_TAG, fmt, ##__VA_ARGS__);                                 \
            break;                                                                  \
        }                                                                           \
    } while (0)

// ---- GENERIC / HOST ----
#else

#include <stdarg.h>

// API
typedef void (*slog_generic_v_fn_t)(slog_level_t level, const char *tag,
                                    const char *fmt, va_list args, void *user);
void slog_generic_set_sink(slog_generic_v_fn_t fn, void *user); // NOLINT

// File scoped registration
#define SLOG_REGISTER(tag_literal)                                                  \
    static const char *const slog_tag__ = (tag_literal)
#define SLOG_TAG (slog_tag__)

void slog_generic_log(slog_level_t level, const char *tag, const char *fmt, ...)
#if defined(__GNUC__) || defined(__clang__)
    __attribute__((format(printf, 3, 4)))
#endif
    ;

// Direct dispatch
#define SLOGV(fmt, ...) slog_generic_log(SLOG_VERBOSE, SLOG_TAG, fmt, ##__VA_ARGS__)
#define SLOGD(fmt, ...) slog_generic_log(SLOG_DEBUG, SLOG_TAG, fmt, ##__VA_ARGS__)
#define SLOGI(fmt, ...) slog_generic_log(SLOG_INFO, SLOG_TAG, fmt, ##__VA_ARGS__)
#define SLOGW(fmt, ...) slog_generic_log(SLOG_WARN, SLOG_TAG, fmt, ##__VA_ARGS__)
#define SLOGE(fmt, ...) slog_generic_log(SLOG_ERROR, SLOG_TAG, fmt, ##__VA_ARGS__)

// Generic dispatch
#define SLOG(level, fmt, ...) slog_generic_log((level), SLOG_TAG, fmt, ##__VA_ARGS__)

#endif
