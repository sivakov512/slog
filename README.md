# Simple C logging library

[![Formatting](https://github.com/sivakov512/slog/actions/workflows/formatting.yml/badge.svg)](https://github.com/sivakov512/slog/actions/workflows/formatting.yml)
[![Checks](https://github.com/sivakov512/slog/actions/workflows/checks.yml/badge.svg)](https://github.com/sivakov512/slog/actions/workflows/checks.yml)

Lightweight, embedded-friendly C logging library with clean colored output and platform-agnostic API.

## Purpose

Have a logger with neat output that works across platforms. Test your embedded libraries on host without suffering - write logs once, run everywhere.

## Features

- 📦 Embedded-friendly (small footprint)
- 🌈 Colored output with timestamps
- 🔧 Platform-agnostic API
- ⚡ Zero dependencies for core functionality

## Output Example

```
I (1234) my_tag: Application started
D (1245) my_tag: Debug information here
W (1256) my_tag: Something might be wrong
E (1267) my_tag: Critical error occurred
```

## Installation and Usage

### Generic (Desktop/Host)

For testing libraries on host or general desktop applications.

Add to your CMakeLists.txt:

<!--x-release-please-start-version-->
```cmake
include(FetchContent)

FetchContent_Declare(
    slog
    GIT_REPOSITORY https://github.com/sivakov512/slog.git
    GIT_TAG        v1.1.0
)
FetchContent_MakeAvailable(slog)

target_link_libraries(your_target slog)
```
<!--x-release-please-end-->

Usage:
```c
#include "slog.h"
#include "generic/slog.h"

#define TAG "my_app"

int main() {
    slog_generic_init();
    
    SLOGI("Hello from generic slog!");
    SLOGD("Debug value: %d", 42);
    SLOGW("Warning message");
    
    return 0;
}
```

### ESP-IDF

For embedded ESP32 applications.

Add to your `idf_component.yml`:

<!--x-release-please-start-version-->
```yaml
dependencies:
  slog:
    git: https://github.com/sivakov512/slog.git
    version: "v1.1.0"
```
<!--x-release-please-end-->

Usage:
```c
#include "slog.h"
#include "esp/slog.h"

#define TAG "my_app"

void app_main() {
    slog_esp_init();
    
    SLOGI("Hello from ESP!");
    SLOGD("Free heap: %d", esp_get_free_heap_size());
    
    // ESP-specific error handling
    RETURN_ON_ERROR(some_esp_function(), "Failed to initialize");
}
```

## Development

```bash
# Check code formatting
make check-format

# Run tests
make test

# Run static analysis
make lint
```
