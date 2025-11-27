// NOLINTBEGIN(readability-magic-numbers)
#include "slog.h"
#include "unity.h"
#include <stdint.h>
#include <string.h>

static char g_last_line[512];
static slog_level_t g_last_level;
static const char *g_last_tag;

static uint32_t g_fake_ts_value;

#define TAG "test_tag"

static uint32_t timestamp_cb() {
  return g_fake_ts_value;
}

static void backend_cb(slog_level_t level, const char *tag, const char *line) {
  g_last_level = level;
  g_last_tag = tag;

  strncpy(g_last_line, line, sizeof(g_last_line));
  g_last_line[sizeof(g_last_line) - 1] = '\0';
}

void setUp() {
  memset(g_last_line, 0, sizeof(g_last_line));
  g_last_level = SLOG_VERBOSE;
  g_last_tag = NULL;

  g_fake_ts_value = 420;

  slog_set_timestamp_cb(timestamp_cb);
  slog_set_backend_cb(backend_cb);
}

void tearDown() {}

static void test_full_output() {
  slog(SLOG_INFO, TAG, "hello");

  TEST_ASSERT_EQUAL(SLOG_INFO, g_last_level);
  TEST_ASSERT_EQUAL_STRING(TAG, g_last_tag);

  TEST_ASSERT_EQUAL_STRING("\x1B[0;32mI (420) test_tag: hello\x1B[0m", g_last_line);
}

static void test_log_level_printed_well(slog_level_t level, char *expected) {
  slog(level, TAG, "msg");

  TEST_ASSERT_NOT_NULL(strstr(g_last_line, expected));
}
#define LOG_LEVEL_CASES(X)                                                                         \
  X(SLOG_VERBOSE, "V (")                                                                           \
  X(SLOG_DEBUG, "D (")                                                                             \
  X(SLOG_INFO, "I (")                                                                              \
  X(SLOG_WARN, "W (")                                                                              \
  X(SLOG_ERROR, "E (")
#define DEF_TEST(level, expected)                                                                  \
  static void test_log_level_printed_well__##level() {                                             \
    test_log_level_printed_well(level, expected);                                                  \
  }
LOG_LEVEL_CASES(DEF_TEST)
#undef DEF_TEST

static void test_prints_tag(void) {
  slog(SLOG_INFO, TAG, "value=%d", 5);

  TEST_ASSERT_EQUAL_STRING(TAG, g_last_tag);
  TEST_ASSERT_NOT_NULL(strstr(g_last_line, TAG));
}

static void test_works_well_with_null_tag(void) {
  slog(SLOG_ERROR, NULL, "oops");

  TEST_ASSERT_NULL(g_last_tag);
  TEST_ASSERT_NULL(strstr(g_last_line, "(null)"));
}

static void test_truncates_too_long_lines() {
  char big[1024];
  memset(big, 'A', sizeof(big) - 1);
  big[sizeof(big) - 1] = '\0';

  slog(SLOG_INFO, TAG, "%s", big);

  size_t len = strlen(g_last_line);
  TEST_ASSERT_LESS_OR_EQUAL(SLOG_LINE_MAX, len);
  TEST_ASSERT_GREATER_OR_EQUAL(SLOG_MSG_MAX, len);
}

static void test_prints_timestamp() {
  g_fake_ts_value = 1234;

  slog(SLOG_INFO, TAG, "msg");

  TEST_ASSERT_NOT_NULL(strstr(g_last_line, "1234"));
}

static void test_macro_produces_the_same_output_as_slog(void (*log_fn)(void), slog_level_t level) {
  char expected[sizeof(g_last_line)];
  slog(level, TAG, "some: %s", "value");
  memcpy(expected, g_last_line, sizeof(expected));

  log_fn();

  TEST_ASSERT_EQUAL_STRING(expected, g_last_line);
}
#define MACRO_CASES(X)                                                                             \
  X(SLOGV, SLOG_VERBOSE)                                                                           \
  X(SLOGD, SLOG_DEBUG)                                                                             \
  X(SLOGI, SLOG_INFO)                                                                              \
  X(SLOGW, SLOG_WARN)                                                                              \
  X(SLOGE, SLOG_ERROR)
#define DEF_TEST(macro, level)                                                                     \
  static void call_##macro() {                                                                     \
    macro("some: %s", "value");                                                                    \
  }                                                                                                \
  static void test_macro_produces_the_same_output_as_slog__##macro() {                             \
    test_macro_produces_the_same_output_as_slog(call_##macro, level);                              \
  }
MACRO_CASES(DEF_TEST)
#undef DEF_TEST

int main(void) {
  UNITY_BEGIN();

  RUN_TEST(test_full_output);
#define RUN(level, expected) RUN_TEST(test_log_level_printed_well__##level);
  LOG_LEVEL_CASES(RUN)
#undef RUN
  RUN_TEST(test_prints_tag);
  RUN_TEST(test_works_well_with_null_tag);
  RUN_TEST(test_truncates_too_long_lines);
  RUN_TEST(test_prints_timestamp);
#define RUN(macro, level) RUN_TEST(test_macro_produces_the_same_output_as_slog__##macro);
  MACRO_CASES(RUN)
#undef RUN

  return UNITY_END();
}

// NOLINTEND(readability-magic-numbers)
