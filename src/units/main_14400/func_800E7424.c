#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Vec2i;

typedef struct {
    u8 pad0[0x64];
    Vec2i pos;
} Object_800E7424;

u8 func_800A6420(Object_800E7424 *obj, Vec2i *pos);
s32 func_800E65C0(Object_800E7424 *obj, Vec2i *pos, s32 arg2);
s32 func_800E66EC(Object_800E7424 *obj);

s32 func_800E7424(Object_800E7424 *obj, Vec2i *pos) {
    Vec2i *current = &obj->pos;
    s32 ok;

    if ((current->y | obj->pos.x) == 0) {
        obj->pos = *pos;
    }
    switch (func_800A6420(obj, pos)) {
        case 0:
        case 1:
            ok = func_800E65C0(obj, current, 0) == 1;
            if (ok) {
                break;
            }
            /* fallthrough */
        default:
            ok = func_800E66EC(obj) == 1;
            if (!ok) {
                return 0;
            }
            break;
    }
    obj->pos = *pos;
    return 1;
}
