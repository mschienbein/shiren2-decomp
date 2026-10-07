#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x1E]; u8 flags_1E; } Obj800A4520;
s32 func_800A44F4(void *ctx, Obj800A4520 *obj);

s32 func_800A4520(void *ctx, Obj800A4520 *obj) {
    if (obj != 0 && ((obj->flags_1E >> 1) & 1)) {
        return 1;
    }
    return func_800A44F4(ctx, obj) == 2;
}
