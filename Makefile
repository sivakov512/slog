.PHONY: build clean test lint check-format fullcheck

CLANG_TIDY ?= clang-tidy
CLANG_TIDY_EXTRAS :=
CLANG_TIDY_HEADER_FILTER ?= $(CURDIR)/(src)/.*

CLANG_FORMAT ?= clang-format

__TESTS_DIR := tests
__TESTS_BUILD_DIR := $(__TESTS_DIR)/build

__UNAME_S := $(shell uname -s)

ifeq ($(__UNAME_S),Darwin)
# Add path to __SDK on MacOS
  __XCRUN := $(shell command -v __XCRUN 2>/dev/null)
  ifneq ($(__XCRUN),)
    __SDK := $(shell __XCRUN --show-__SDK-path 2>/dev/null)
    ifneq ($(__SDK),)
      CLANG_TIDY_EXTRAS += --extra-arg=-isysroot$(__SDK)
    endif
  endif
endif

build:
	cmake -S ${__TESTS_DIR} -B ${__TESTS_BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
	cmake --build ${__TESTS_BUILD_DIR} -j

clean:
		rm -rf ${__TESTS_BUILD_DIR}

test: build
	ctest --test-dir ${__TESTS_BUILD_DIR} --output-on-failure -V

lint: build
	find . \
    	\( -type d \( -name build -o -name ".cache" -o -name ".git" \) -prune \) \
    	-o \
    	\( -type d -name platform -prune \) \
    	-o \
    	-type f \( -name "*.c" -o -name "*.cpp" \) \
        -print0 \
		| xargs -0 ${CLANG_TIDY} ${CLANG_TIDY_EXTRAS} --header-filter='$(CLANG_TIDY_HEADER_FILTER)' -p ${__TESTS_BUILD_DIR}

check-format:
	find . \
		\( -type d \( -name build -o -name ".cache" -o -name ".git" \) -prune \) \
		-o \
    	-type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) \
        -print0 \
		| xargs -0 ${CLANG_FORMAT} --dry-run --Werror

fullcheck: test lint check-format
