# Slog - simple cross-platform logging macros for C

[![Formatting](https://github.com/sivakov512/slog/actions/workflows/formatting.yml/badge.svg)](https://github.com/sivakov512/slog/actions/workflows/formatting.yml)
[![Checks](https://github.com/sivakov512/slog/actions/workflows/checks.yml/badge.svg)](https://github.com/sivakov512/slog/actions/workflows/checks.yml)

One API. Native backends. No unnecessary overhead.
• ESP-IDF → maps directly to ESP_LOGx
• Host / Generic → user-provided sink callback
• Silent by default on host
• No extra formatting layer on ESP

## Basic usage

In each .c file:

```c
#include "slog.h"

SLOG_REGISTER("my_module");

SLOGI("hello %d", 42);
SLOGW("something odd");
SLOGE("failed: %s", "reason");
```

Dynamic level:

```c
SLOG(SLOG_WARN, "value=%d", v);
```

SLOG_REGISTER("tag") must be used once per file (file scope).

## Backends

### ESP-IDF

Enabled automatically if:
• ESP_PLATFORM is defined, or
• SLOG_BACKEND_ESP is defined.

Mappings are direct:
• SLOGI → ESP_LOGI
• SLOGE → ESP_LOGE
• SLOG(level, ...) → correct ESP_LOGx

No extra buffers. No additional stack usage.

Logging behavior (levels, filtering, formatting) is fully controlled by ESP-IDF.

### Host / Generic

Install a sink:

```c
#include "slog.h"
#include <stdio.h>

static void sink(slog_level_t level,
                 const char *tag,
                 const char *fmt,
                 va_list args,
                 void *user)
{
  (void)user;
  fprintf(stderr, "[%d] %s: ", (int)level, tag ? tag : "");
  vfprintf(stderr, fmt, args);
  fputc('\n', stderr);
}

int main(void) {
  slog_generic_set_sink(sink, NULL);

  SLOG_REGISTER("host");
  SLOGI("hello");
}
```

If no sink is set, logs are ignored.

## Installation (CMake)

<!--x-release-please-start-version-->

```cmake
include(FetchContent)

FetchContent_Declare(
  slog
  GIT_REPOSITORY https://github.com/sivakov512/slog.git
  GIT_TAG        v1.1.0
)
FetchContent_MakeAvailable(slog)

target_link_libraries(your_target PRIVATE slog)
```

<!--x-release-please-end-->

## Notes

    •	Designed for embedded libraries tested on host.
    •	No default output on desktop — applications decide where logs go.
    •	Keep it simple: macros in your code, native logging underneath.
