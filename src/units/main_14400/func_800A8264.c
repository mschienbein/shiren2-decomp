#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct {
    u8 pad0[0x1C];
    u16 flags1C;
} Obj800A8264;

s32 func_800A8264(Obj800A8264 *obj) {
    s32 flag = obj->flags1C & 8;

    return flag != 0;
}
