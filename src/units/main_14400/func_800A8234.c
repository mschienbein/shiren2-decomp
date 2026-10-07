#include "common.h"

typedef unsigned short u16;
typedef struct { char pad0[0x1C]; u16 flags1C; } Obj;
s32 func_800A8234(Obj *obj) {
    s32 f = obj->flags1C & 0x10;
    return f != 0;
}
