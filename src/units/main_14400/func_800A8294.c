#include "common.h"

typedef unsigned short u16;
typedef struct { char pad[0x1C]; u16 flags1C; } Obj;
s32 func_800A8294(Obj *obj) {
    s32 r = 0;
    if (obj->flags1C & 4) r = 1;
    return r;
}
