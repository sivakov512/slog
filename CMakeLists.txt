cmake_minimum_required(VERSION 3.16)

set(PUBLIC_HEADERS include)

set(SRCS src/slog.c)
project(slog C)

add_library(slog STATIC ${SRCS} ${GENERIC_SRCS})
target_include_directories(slog PUBLIC ${PUBLIC_HEADERS})
