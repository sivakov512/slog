#include "slog.h"
#include <stdio.h>

static slog_generic_v_fn_t g_sink = NULL;
static void *g_user = NULL;

void slog_generic_set_sink(slog_generic_v_fn_t fn, void *user) {
    g_sink = fn;
    g_user = user;
}

void slog_generic_log(slog_level_t level, const char *tag, const char *fmt, ...) {
    slog_generic_v_fn_t sink = g_sink;
    void *user = g_user;

    if (sink == NULL) {
        return;
    }

    va_list args;
    va_start(args, fmt);

    va_list copy;
    va_copy(copy, args);
    sink(level, tag, fmt, copy, user);
    va_end(copy);

    va_end(args);
}
