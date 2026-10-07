#include "common.h"

char *func_80083C90(char *dst, char *src)
{
    char *d = dst;

    while ((*d++ = *src++) != 0) {
    }
    return dst;
}
