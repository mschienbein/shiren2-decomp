#include "common.h"
typedef signed char s8;
typedef unsigned char u8;
s32 func_8005E1B4(const s8 *text, s32 *position) {
    s32 current = *position;
    s32 next = current + 1;
    s8 first = text[current];
    s32 value = first & 0xFF;
    *position = next;
    if ((first & 0xF0) == 0xF0) {
        s32 second = (u8)text[next];
        *position = current + 2;
        return (value << 8) | second;
    } else {
        return value;
    }
}
