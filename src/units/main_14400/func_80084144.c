#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
extern u8 D_8014D394[];
typedef u32 size_t;
s32 func_80083D8C(void *left, void *right, u32 count);
char *func_80083CC8(char *dst, char *src, u32 count);
char *func_80083C90(char *dst, char *src);
size_t func_80032D70(const char *s);
u8 *func_80084144(u8 *str, u8 *out, s32 *found) {
    u8 *start = str;
    u8 *p = str;
    *found = 0;
    do {
        if (func_80083D8C(p, D_8014D394, 2) != 0) {
            p++;
        } else {
            *found = 1;
            if (p == start) {
                *out = 0;
            } else {
                func_80083CC8((char *)out, (char *)start, (u32)(p - start));
            }
            str = p + 2;
            break;
        }
    } while (*p != 0);
    if (str == start) {
        func_80083C90((char *)out, (char *)str);
        str += func_80032D70((const char *)str);
    }
    return str;
}
