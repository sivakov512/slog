cmake_minimum_required(VERSION 3.16)

set(PUBLIC_HEADERS include)
set(GENERIC_SRCS src/generic.c)

if(COMMAND idf_component_register)
    idf_component_register(
        INCLUDE_DIRS ${PUBLIC_HEADERS}
        REQUIRES
            log
    )
else()
    project(slog C)

    add_library(slog STATIC ${GENERIC_SRCS})
    target_include_directories(slog PUBLIC ${PUBLIC_HEADERS})
endif()
