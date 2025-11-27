cmake_minimum_required(VERSION 3.16)

set(LIBNAME slog)
set(TESTNAME slog_tests)

project(slog_tests C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup Unity
include(FetchContent)

FetchContent_Declare(
    Unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.1
)
FetchContent_MakeAvailable(Unity)

if(NOT TARGET unity)
    add_library(unity STATIC ${Unity_SOURCE_DIR}/src/unity.c)
    target_include_directories(unity PUBLIC ${Unity_SOURCE_DIR}/src)
endif()

enable_testing()

# Add project
add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/../ ${CMAKE_BINARY_DIR}/${LIBNAME})


# Add tests
set(TEST_SRCS test_slog.c)

add_executable(${TESTNAME} ${TEST_SRCS})
target_link_libraries(${TESTNAME} PRIVATE unity ${LIBNAME})

set_target_properties(${TESTNAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
