#include "slog.h"
#include "unity.h"

#define TEST_SLOG_MAX 512

typedef struct {
    void *user;
    slog_level_t level;
    const char *tag;
    char msg[TEST_SLOG_MAX];
} captured_args_t;

static captured_args_t g_captured_args;
static uint8_t g_dummy_user = 3;

SLOG_REGISTER("test_generic");

void capture_sink(slog_level_t level, const char *tag, const char *fmt, va_list args,
                  void *user) {
    g_captured_args = (captured_args_t) {.user = user, .level = level, .tag = tag};
    vsnprintf(g_captured_args.msg, sizeof(g_captured_args.msg), fmt, args);
    g_captured_args.msg[sizeof(g_captured_args.msg) - 1] = '\0';
}

void setUp() {
    slog_generic_set_sink(capture_sink, &g_dummy_user);
}

void tearDown() {}

static void test_args_passed_well() {
    SLOG(SLOG_INFO, "hello");

    TEST_ASSERT_EQUAL(SLOG_INFO, g_captured_args.level);
    TEST_ASSERT_EQUAL_STRING("test_generic", g_captured_args.tag);
    TEST_ASSERT_EQUAL_STRING("hello", g_captured_args.msg);
    TEST_ASSERT_EQUAL_PTR(&g_dummy_user, g_captured_args.user);
}

static void test_formatting_works_fine() {
    SLOG(SLOG_INFO, "hello %s!!%d", "motherfucker", 1);

    TEST_ASSERT_EQUAL_STRING("hello motherfucker!!1", g_captured_args.msg);
}

static void test_macro_produces_expected_output(void (*log_fn)(void),
                                                slog_level_t level) {
    log_fn();

    TEST_ASSERT_EQUAL(level, g_captured_args.level);
    TEST_ASSERT_EQUAL_STRING("test_generic", g_captured_args.tag);
    TEST_ASSERT_EQUAL_STRING("yoman", g_captured_args.msg);
    TEST_ASSERT_EQUAL_PTR(&g_dummy_user, g_captured_args.user);
}
#define MACRO_CASES(X)                                                              \
    X(SLOGV, SLOG_VERBOSE)                                                          \
    X(SLOGD, SLOG_DEBUG)                                                            \
    X(SLOGI, SLOG_INFO)                                                             \
    X(SLOGW, SLOG_WARN)                                                             \
    X(SLOGE, SLOG_ERROR)
#define DEF_TEST(macro, level)                                                      \
    static void call_##macro() {                                                    \
        macro("yoman");                                                             \
    }                                                                               \
    static void test_macro_produces_expected_output__##macro() {                    \
        test_macro_produces_expected_output(call_##macro, level);                   \
    }
MACRO_CASES(DEF_TEST)
#undef DEF_TEST

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_args_passed_well);
    RUN_TEST(test_formatting_works_fine);
#define RUN(macro, level) RUN_TEST(test_macro_produces_expected_output__##macro);
    MACRO_CASES(RUN)
#undef RUN

    return UNITY_END();
}
