#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { s32 x, y; } Pos;
typedef struct { u8 value; } Dir;

typedef struct {
    Pos pos;
    Dir dir;
    u8 pad9[0x1C - 9];
    u16 flags_1C;
} Unit800A7824;

extern u32 func_800B1C6C(Pos *pos);
extern s32 func_800A4CC4(void *p, void *v, void *a);
extern s32 func_800B4F74(Pos *pos);
extern s32 func_800B56F0(void *object);
extern void func_800A2758(Pos *p, Dir d);
extern u32 func_80048AC8(void);
extern void func_80046B3C(void *obj, Dir d);
extern s32 func_800A6FD0(Unit800A7824 *self);
extern void func_800E2298(void *arg0);
extern s32 func_80049CB4(s32 id, ...);
extern void func_800A59A4(Unit800A7824 *self);
extern s32 func_800A4360(void *a, void *b);
extern void func_800A5A70(void *object, u32 value);
extern s32 func_800B48C0(Pos *a, Unit800A7824 *b);

/* Slide the unit up to steps cells along its facing; returns the number of cells moved. */
s32 func_800A7824(Unit800A7824 *unit, u8 steps) {
    Pos start;
    Pos cur;
    Pos *next;
    s32 moved;
    s32 stuck = 0;

    if ((unit->flags_1C & 0x40) || (unit->flags_1C & 1)) {
        stuck = 1;
    }
    if (stuck) {
        return 0;
    }
    start.x = unit->pos.x;
    start.y = unit->pos.y;
    moved = 0;
    {
        /* The byte countdown terminates when decrementing zero wraps to 0xFF. */
        s32 stop = 0xFF;
        cur.x = unit->pos.x;
        cur.y = unit->pos.y;
        next = &cur;
        while (1) {
            s32 blocked;

            if (--steps == stop) {
                break;
            }
            blocked = 0;

            if (!(func_800B1C6C(&cur) & 0x80) || !func_800A4CC4(unit, next, &unit->dir) ||
                func_800B4F74(next) || func_800B56F0(next)) {
                blocked = 1;
            }
            if (blocked) {
                break;
            }
            func_800A2758(&cur, unit->dir);
            moved++;
        }
    }
    if (func_80048AC8()) {
        steps = moved;
        while (steps-- != 0) {
            func_80046B3C(unit, unit->dir);
            func_800A2758(&unit->pos, unit->dir);
            if (func_800A6FD0(unit)) {
                func_800E2298(unit);
            }
            func_80049CB4(0xD7, unit);
        }
    }
    if (moved > 0) {
        unit->pos = start;
        func_800A59A4(unit);
        unit->pos = cur;
        func_800A5A70(unit, func_800A4360(unit, &cur));
        func_800B48C0(&cur, unit);
    }
    return moved;
}
