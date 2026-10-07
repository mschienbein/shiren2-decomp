#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
u16 func_80083E00(u8);
u8 *func_80083EE4(u8 ch, u8 *dst) {
    u16 code = func_80083E00(ch);
    if (code >= 0x100) {
        *dst++ = code >> 8;
    }
    *dst = code;
    return dst + 1;
}
