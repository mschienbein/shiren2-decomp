#include "common.h"

typedef unsigned char u8;

typedef struct {
    s32 x;
    s32 y;
} Pos8010AC94;

typedef struct {
    Pos8010AC94 pos;
    char pad8[0x54 - 0x8];
    u8 flags54;
    char pad55[0xC0 - 0x55];
    s32 hasTarget;
    Pos8010AC94 target;
    Pos8010AC94 lastTarget;
} Obj8010AC94;

extern s32 func_800A692C(Obj8010AC94 *obj, s32 kind);
extern void *func_800B4D80(Pos8010AC94 *pos);
extern s32 func_800A251C(Pos8010AC94 *a, Pos8010AC94 *b);
extern s32 func_800E65C0(Obj8010AC94 *obj, Pos8010AC94 *pos, s32 arg2);
extern void *func_8010A9E4(Obj8010AC94 *obj, Pos8010AC94 *pos);
extern s32 func_800EF184(Obj8010AC94 *obj, u8 arg1);

s32 func_8010AC94(Obj8010AC94 *obj, u8 arg1) {
    Pos8010AC94 start;
    Pos8010AC94 *startp = &start;
    s32 active;

    active = 0;
    startp->x = obj->pos.x;
    startp->y = obj->pos.y;
    if (func_800A692C(obj, 0x12) == 0) {
        active = arg1 != 1;
    }
    if (active && obj->hasTarget != 0) {
        if (func_800B4D80(&obj->target) != 0) {
            if (func_800A251C(startp, &obj->target) != 0) {
                obj->flags54 |= 4;
                return 0;
            }
            if (func_800E65C0(obj, &obj->target, 0) != 0) {
                return 1;
            }
            obj->lastTarget = obj->target;
        }
        obj->hasTarget = 0;
    }
    if (active && func_8010A9E4(obj, &obj->target) != 0) {
        obj->hasTarget = 1;
        if (func_800A251C(&obj->target, &start) != 0) {
            obj->flags54 |= 4;
            return 0;
        }
        return func_800E65C0(obj, &obj->target, 0);
    }
    return func_800EF184(obj, arg1);
}
