#include "common.h"

/* libultra libc string.c: strchr, strlen, memcpy */

typedef u32 size_t;

const char *func_80032D30(const char *s, s32 c)
{
    const char ch = c;

    while (*s != ch) {
        if (*s == '\0') {
            return 0;
        }
        s++;
    }
    return (const char *)s;
}

size_t func_80032D70(const char *s)
{
    const char *sc = s;

    while (*sc != '\0') {
        sc++;
    }
    return (size_t)(sc - s);
}

void *func_80032D94(void *s1, const void *s2, size_t n)
{
    char *su1 = (char *)s1;
    const char *su2 = (const char *)s2;

    while (n > 0) {
        *su1++ = *su2++;
        n--;
    }
    return (void *)s1;
}
