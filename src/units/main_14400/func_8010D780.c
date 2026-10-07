#include "common.h"

typedef unsigned char u8;
typedef struct { u8 pad0[2]; u8 flags_2; } Obj;

extern s32 func_8010BEC4(Obj *obj, u8 id);

s32 func_8010D780(Obj *obj, s32 kind) {
    if (kind == 2 && (u8)func_8010BEC4(obj, 0x6D)) {
        return 1;
    }
    if (kind == 3 || kind == 0xB) {
        return 1;
    }
    if (kind == 0 && !(obj->flags_2 & 4)) {
        return 1;
    }
    if (kind == 1 && (obj->flags_2 & 4)) {
        return 1;
    }
    if (kind == 0x21 && (u8)func_8010BEC4(obj, 0x78)) {
        return 1;
    }
    return 0;
}
