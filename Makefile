.PHONY: build test clean lint check-format

CLANG_TIDY ?= clang-tidy
CLANG_TIDY_EXTRAS :=
CLANG_FORMAT ?= clang-format

TESTS_DIR := tests
TESTS_BUILD_DIR := $(TESTS_DIR)/build

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
# Add path to SDK on MacOS
  XCRUN := $(shell command -v xcrun 2>/dev/null)
  ifneq ($(XCRUN),)
    SDK := $(shell xcrun --show-sdk-path 2>/dev/null)
    ifneq ($(SDK),)
      CLANG_TIDY_EXTRAS += --extra-arg=-isysroot$(SDK)
    endif
  endif
endif

build:
	cmake -S ${TESTS_DIR} -B ${TESTS_BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
	cmake --build ${TESTS_BUILD_DIR} -j

test: build
	ctest --test-dir ${TESTS_BUILD_DIR} --output-on-failure -V

clean:
	rm -rf ${TESTS_BUILD_DIR}

lint: build
	find . \
    	\( -type d \( -name build -o -name ".cache" -o -name ".git" \) -prune \) \
    	-o \
    	\( -type d -name platform -prune \) \
    	-o \
    	-type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) \
        -print0 \
		| xargs -0 ${CLANG_TIDY} ${CLANG_TIDY_EXTRAS} -p ${TESTS_BUILD_DIR}

check-format:
	find . \
		\( -type d \( -name build -o -name ".cache" -o -name ".git" \) -prune \) \
		-o \
    	-type f \( -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" \) \
        -print0 \
		| xargs -0 ${CLANG_FORMAT} --dry-run --Werror
