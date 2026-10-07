#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x1C]; u16 flags; } Obj800429E0;
void *func_800A8CB0(s32 cell);
s32 func_800429E0(u8 id) {
    Obj800429E0 *obj = func_800A8CB0(id);
    if (obj->flags & 0x40) return 1;
    return 0;
}
