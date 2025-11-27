cmake_minimum_required(VERSION 3.16)

set(PUBLIC_HEADERS include)

set(SRCS src/slog.c)
set(ESP_SRCS src/platform/esp.c)
set(GENERIC_SRCS src/platform/generic.c)

if(COMMAND idf_component_register)
    idf_component_register(
        SRCS ${SRCS} ${ESP_SRCS}
        INCLUDE_DIRS ${PUBLIC_HEADERS}
        REQUIRES
            log
    )
else()
    project(slog C)

    add_library(slog STATIC ${SRCS} ${GENERIC_SRCS})
    target_include_directories(slog PUBLIC ${PUBLIC_HEADERS})
endif()
