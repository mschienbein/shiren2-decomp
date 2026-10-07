#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

s32 func_800E776C(void *obj, u8 mode);
/* Ally slot +0xC4 supplies a byte mode, unused by this override. */
s32 func_8010B4F0(void *obj, u8 mode) {
    return func_800E776C(obj, 3);
}
