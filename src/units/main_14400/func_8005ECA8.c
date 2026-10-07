#include "common.h"
typedef void *(*CopyCallback)(void *, const char *, u32);
extern s32 func_8005E714(CopyCallback, char *, const char *, void *);
extern void *func_8005EC74(void *dst, const char *src, u32 length);
s32 func_8005ECA8(char *buf, const char *fmt, void *args) {
    s32 n = func_8005E714(func_8005EC74, buf, fmt, args);
    if (n >= 0) buf[n] = 0;
    return n;
}
