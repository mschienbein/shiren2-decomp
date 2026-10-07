#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { u8 pad0[0x1C]; u16 flags; } Obj800A814C;

s32 func_800A814C(Obj800A814C *obj) {
    s32 bit = obj->flags & 0x200;
    return bit != 0;
}
