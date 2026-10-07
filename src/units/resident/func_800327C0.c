#include "common.h"

typedef unsigned int size_t;

extern void *func_80032D94(void *dst, const void *src, size_t count);
extern s32 func_800354B0(char *(*prout)(char *, const char *, size_t), char *dst, const char *fmt, void *args);

char *func_80032818(char *dst, const char *src, size_t count);

s32 func_800327C0(char *dst, const char *fmt, ...)
{
    s32 written;

    written = func_800354B0(func_80032818, dst, fmt, __builtin_next_arg(fmt));
    if (written >= 0) {
        dst[written] = 0;
    }
    return written;
}

char *func_80032818(char *dst, const char *src, size_t count)
{
    func_80032D94(dst, src, count);
    return dst + count;
}
