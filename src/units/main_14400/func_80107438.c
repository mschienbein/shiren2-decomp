#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct Actor {
    Pos pos;
    u8 pad08[0x2B];
    u8 x33;
    u8 pad34[0x24];
    struct Actor *x58;
    u8 pad5C[0x44];
    s32 xA0;
    Pos xA4;
} Actor;
extern s32 func_800A692C(void *object, s32 code);
extern s32 func_800F3310(void *obj);
extern s32 func_800A251C(Pos *x, Pos *y);
extern s32 func_800E1CD4(Actor *obj, s32 value);
extern s32 func_800E65C0(Actor *unit, Pos *target, s32 mode);
extern u8 func_800A6420(Actor *obj, Actor *target);
extern void func_800F06E4(Actor *obj);
extern void *func_800A65E4(Dir *p, Actor *q, void *target);
extern void func_800A665C(Actor *obj, u8 *value);
extern s32 func_800A67DC(Actor *u, Actor *o, s32 force, s32 apply);
extern s32 func_800F1024(Actor *object);
extern s32 func_800E8350(Actor *unit);
extern s32 func_800E0F40(Actor *obj);
extern s32 func_800E7104(Actor *unit);
extern s32 func_800E7AA8(Actor *obj, s32 a1);

s32 func_80107438(Actor *s) {
    Actor *target = s->x58;
    Actor *other = target;
    s32 notOne, notThree;

    notOne = s->xA0 != 1;
    if (!notOne) {
        s->xA4 = s->pos;
        return 0;
    }
    if (func_800A692C(s, 0x12)) {
        return func_800F3310(s);
    }
    notThree = s->xA0 != 3;
    if (!notThree) {
        s32 moved = func_800A251C(&s->xA4, &s->pos) != 1;
        if (moved) {
            if (func_800E1CD4(s, 0x10)) {
                return func_800E8350(s);
            }
            return func_800E65C0(s, &s->xA4, 0);
        }
        if (func_800A6420(s, target) == 0) {
            func_800F06E4(s);
        } else {
            Dir dir;
            func_800A65E4(&dir, s, target);
            func_800A665C(s, &dir.value);
        }
        return 0;
    }
    switch (func_800A6420(s, target)) {
    case 0:
        if (s->x33 < 2) {
            func_800F06E4(s);
            return 0;
        }
        break;
    case 1:
    case 2: {
        s32 ok = 0;
        if (func_800A67DC(s, other, 0, 1)) {
            ok = func_800F1024(s) != 0;
        }
        if (ok) {
            func_800F06E4(s);
            return 0;
        }
        break;
    }
    }
    if (func_800E1CD4(s, 0x10)) {
        return func_800E8350(s);
    }
    if ((u8)func_800E0F40(s) != 1) {
        return func_800E7AA8(s, 3);
    }
    return func_800E7104(s);
}
