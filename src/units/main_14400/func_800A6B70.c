#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { s32 x, y; } Pair;

Pair func_800A694C(u8 *center, u8 *bounds, u8 filter, s32 check);

/* Struct-return forwarder: the hidden result pointer is passed through and returned. */
Pair func_800A6B70(u8 *src, u8 mode, s32 arg) {
    return func_800A694C(src, src + 0xC, mode, arg);
}
