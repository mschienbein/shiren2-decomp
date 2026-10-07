#include "common.h"

extern void *func_80032D94(void *dst, const void *src, u32 count);

/* The formatter callback returns the next destination byte. */
char *func_8005EC74(char *base, const char *src, u32 count)
{
    func_80032D94(base, src, count);
    return base + count;
}
