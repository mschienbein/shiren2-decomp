#include "common.h"

typedef unsigned char u8;
typedef struct { s32 y; s32 x; } Pos;
typedef struct { u8 pad0[0x1E]; u8 flags_1E; } Obj;
Obj *func_800B4928(Pos *pos);
s32 func_80041600(s32 x, s32 y) {
    Pos pos;
    Obj *obj;
    pos.x = x;
    pos.y = y;
    obj = func_800B4928(&pos);
    if (obj == 0) {
        return 0;
    }
    return (obj->flags_1E >> 2) & 1;
}
