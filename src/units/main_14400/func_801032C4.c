#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef struct { s32 x, y; } Pos;
typedef struct { u8 value; } Dir;
typedef struct Obj {
    /* 0x00 */ Pos position;
    /* 0x08 */ Dir direction;
    /* 0x09 */ u8 pad9[0xA0 - 0x09];
    /* 0xA0 */ Pos destination;
} Obj;

extern u8 D_80147620[];
extern s8 D_80148440[];
extern u8 D_80156A09;

extern s32 func_800F0EC4(Obj *obj);
extern void *func_800A2594(Pos *out, void *from, Dir dir);
extern void *func_800B4928(Pos *pos);
extern void *func_800B4D80(Pos *pos);
extern u32 func_800B1C6C(Pos *pos);
extern unsigned char func_800C57A0(void *object);
extern s32 func_800E20CC(void *obj);
extern void *func_800A7DEC(void *obj);
extern s32 func_800A4CC4(void *obj, void *value, void *direction);
extern void func_800A4EC0(void *state, void *value);
extern void *func_800A6538(void *out_direction, void *obj, void *target);
extern void func_800A665C(Obj *obj, u8 *value);
extern void func_800A2F80(unsigned char *self, s32 step);
extern s32 func_801031D8(u8 *self, Pos *pos, Dir *dir);
extern void func_800A2758(Pos *p, Dir d);
extern s32 func_800B56F0(void *object);
extern s32 func_800B5300(void *pos, void *actor, u8 amount);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800497F0(s32 id, ...);
extern void func_800AD868(Pos *position);
extern s32 func_800AD8AC(void *item, Pos *position);
extern char *func_800A7DE4(Obj *obj);
extern s32 func_800A4EFC(void *p, void *a);

static inline void copyDirection(Dir *dest, Dir src) { *dest = src; }
static inline void copyPosition(Pos *dest, Pos *src) { dest->x = src->x; dest->y = src->y; }

s32 func_801032C4(Obj *obj) {
    Pos origin, ahead, sideA, sideB;
    Dir direction, left, right, action, turned;
    void *item;
    s32 changed, canTurn;
    Obj *other;
    s8 *offsets;

    if (func_800F0EC4(obj)) {
        return 0;
    }
    copyDirection(&direction, obj->direction);
    func_800A2594(&origin, &obj->position, direction);
    changed = 0;
    canTurn = 0;
    other = func_800B4928(&origin);
    item = func_800B4D80(&origin);
    func_800A2594(&ahead, &origin, direction);
    if (!(func_800B1C6C(&ahead) & 0x800) && (func_800C57A0(D_80147620) & 1)) {
        canTurn = func_800E20CC(obj) == 0;
    }
    if (canTurn) {
        turned.value = (direction.value + 2) & 7;
        func_800A2594(&sideA, &origin, turned);
        if (func_800B1C6C(&sideA) & 0x800) {
            left.value = (direction.value - 1) & 7;
            if (func_800A4CC4(obj, func_800A7DEC(obj), &left)) {
                changed = 1;
                direction.value = (direction.value - 1) & 7;
            }
        } else {
            turned.value = (direction.value - 2) & 7;
            func_800A2594(&sideB, &origin, turned);
            if (func_800B1C6C(&sideB) & 0x800) {
                right.value = (direction.value + 1) & 7;
                if (func_800A4CC4(obj, func_800A7DEC(obj), &right)) {
                    changed = 1;
                    direction.value = (direction.value + 1) & 7;
                }
            }
        }
        if (changed) {
            func_800A4EC0(obj, &direction);
            func_800A6538(&action, obj, &origin);
            func_800A665C(obj, &action.value);
            return 1;
        }
    }
    direction = obj->direction;
    offsets = &D_80148440[(func_800C57A0(D_80147620) & 1) ? 0 : 7];
    changed = 0;
    do {
        func_800A2F80(&direction.value, *offsets++);
        if (func_801031D8((u8 *)obj, &origin, &direction)) {
            break;
        }
        changed++;
    } while (changed < 7);
    if (changed == 7) {
        return 0;
    }
    func_800A2758(&obj->destination, direction);
    if (other) {
        func_800A4EC0(other, &direction);
        copyPosition(&ahead, &other->position);
        if (func_800B56F0(&ahead)) {
            func_800B5300(&ahead, 0, D_80156A09);
            func_800497F0(0x107, func_80049CB4(0xDA, &ahead));
        }
    } else if (item) {
        func_800AD868(&origin);
        func_800AD8AC(item, &obj->destination);
    }
    func_800A4EFC(obj, func_800A7DE4(obj));
    func_800A665C(obj, &direction.value);
    return 1;
}
