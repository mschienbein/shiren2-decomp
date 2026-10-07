#include "common.h"

typedef unsigned char u8;

typedef struct { u8 pad0[0xC]; u8 flags; } Obj80128B54;

s32 func_80128B54(Obj80128B54 *obj) {
    s32 bit = obj->flags & 2;
    return bit != 0;
}
