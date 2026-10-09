#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[4];
    u8 mode;
    u8 flags;
    u8 pad6;
    signed char index;
} Obj80094B3C;

void func_80094BC8(unsigned char *arg0);

void func_80094B3C(Obj80094B3C *obj, s32 mode)
{
    obj->mode = mode;
    if (mode == 1) {
        obj->flags |= 0x10;
    } else {
        obj->flags &= ~0x10;
    }
    obj->index = -1;
    func_80094BC8((unsigned char *)obj);
}
