#include "common.h"

typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;

s32 func_80083D54(u8 *s1, u8 *s2) {
    while (*s1 == *s2) {
        if (*s1++ == 0) {
            return 0;
        }
        s2++;
    }
    return *s1 - *s2;
}
