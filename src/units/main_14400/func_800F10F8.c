#include "common.h"
typedef struct { unsigned char kind, variant, field_02, flags, row, field_05, field_06, field_07, mode; signed char coordinates[2], status; } SelectionRecord;
extern SelectionRecord D_80142F18;
typedef unsigned char u8;
typedef struct { s32 x; s32 y; } Vec2;
typedef struct { u8 value; } Dir;
typedef struct { Vec2 position; } Obj80100160;
extern u8 func_800A6420(Obj80100160 *obj, void *target);
extern s32 func_800A692C(Obj80100160 *, s32);
extern void *func_800A6CF0(Obj80100160 *);
extern s32 func_800A4520(void *ctx, void *obj);
extern s32 func_800E20CC(void *arg0);
extern s32 func_800A67DC(Obj80100160 *obj, void *target, s32 force, s32 apply);
extern Dir *func_800A22B8(Dir *out, Vec2 *from, Vec2 *to);
extern void func_800A665C(Obj80100160 *obj, u8 *value);

s32 func_800F10F8(Obj80100160 *object, void *target, s32 force, s32 apply, s32 override)
{
    Vec2 from;
    Vec2 to;
    Dir direction;
    u8 state = func_800A6420(object, target);
    if (override == 0 && func_800A692C(object, 18) != 0) {
        if (state == 0 || func_800A4520(object, func_800A6CF0(object)) != 0) {
            return 1;
        }
        return 2;
    } else {
        s32 blocked = 0;
        Vec2 *origin;
        if (func_800E20CC(object) != 0 || (D_80142F18.mode ^ 79) == 0) {
            blocked = 1;
        }
        if (blocked != 0) {
            return 0;
        }
        if (state == 3) {
            return 2;
        }
        origin = &from;
        origin->x = object->position.x;
        origin->y = object->position.y;
        to.x = ((Vec2 *)target)->x;
        to.y = ((Vec2 *)target)->y;
        if (func_800A67DC(object, target, force, apply) != 0) {
            return 0;
        }
        func_800A22B8(&direction, origin, &to);
        func_800A665C(object, &direction.value);
        return 2;
    }
}
