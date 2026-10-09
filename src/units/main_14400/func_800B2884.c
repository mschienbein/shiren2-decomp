#include "common.h"

typedef unsigned char u8;

typedef short s16;
typedef struct {
    s16 delta;
    s16 index;
    void *func;
} VTableEntry;

typedef struct {
    s32 x;
    s32 y;
} Pos800B2884;

typedef struct {
    Pos800B2884 cur;
    Pos800B2884 min;
    Pos800B2884 max;
} Iter800B2884;

typedef struct {
    u8 pad0[3];
    u8 state3;
} Cell800B2884;

typedef struct {
    u8 pad0[9];
    u8 kind9;
    u8 padA[0x14];
    u8 flags1E;
    u8 pad1F[5];
    VTableEntry *vtable24;
} Obj800B2884;

/* Two map corners: D_801429C0.first.x/C4 = min x/y, D_801429C0.last.x/CC = max x/y. */
typedef struct { Pos800B2884 first, last; } Rectangle;
extern Rectangle D_801429C0;
extern Pos800B2884 *func_800A3610(Pos800B2884 *out, Iter800B2884 *iter);
extern Cell800B2884 *func_800B4D80(Pos800B2884 *pos);
extern u32 func_800B1C6C(Pos800B2884 *pos);
extern Obj800B2884 *func_800B4928(Pos800B2884 *pos);
extern void func_800A5A70(Obj800B2884 *obj, u32 arg1);
extern s32 func_800E1CD4(Obj800B2884 *obj, s32 arg1);
extern s32 func_80049CB4(s32 command, ...);
/* ODD_C: Field accessors preserve the iterator's original promoted temporaries. */
static inline s32 first_x(Rectangle *r) { return r->first.x; }
static inline s32 first_y(Rectangle *r) { return r->first.y; }
static inline s32 last_x(Rectangle *r) { return r->last.x; }
static inline s32 last_y(Rectangle *r) { return r->last.y; }

static inline s32 iterValid(Iter800B2884 *iter) {
    return iter->cur.x <= iter->max.x;
}

/* ODD_C: The setter keeps the byte value's promoted constant in loop scope. */
static inline void set_state(Cell800B2884 *cell, s32 state) { cell->state3 = state; }

void func_800B2884(void) {
    Iter800B2884 iter;
    Pos800B2884 pos;
    Cell800B2884 *cell;
    Obj800B2884 *obj;
    VTableEntry *entry;
    s32 affected;
    s32 minX;
    s32 minY;
    s32 maxX;
    s32 maxY;

    minX = first_x(&D_801429C0);
    minY = first_y(&D_801429C0);
    maxX = last_x(&D_801429C0);
    maxY = last_y(&D_801429C0);
    pos.x = minX;
    pos.y = minY;
    iter.min = pos;
    iter.cur = iter.min;
    pos.x = maxX;
    pos.y = maxY;
    iter.max = pos;
    while (iterValid(&iter)) {
        func_800A3610(&pos, &iter);
        cell = func_800B4D80(&pos);
        if (cell != 0) {
            set_state(cell, 2);
        }
        if (func_800B1C6C(&pos) & 0x80) {
            continue;
        }
        obj = func_800B4928(&pos);
        if (obj == 0) {
            continue;
        }
        if ((obj->kind9 & 0xF) != 1) {
            continue;
        }
        func_800A5A70(obj, 2);
        affected = (obj->flags1E & 0x7C) && func_800E1CD4(obj, 0xD);
        if (affected) {
            entry = &obj->vtable24[18];
            ((s32 (*)(void *, s32, s32, u8, s32))entry->func)((u8 *)obj + entry->delta, 1, 0xD, 0, 0);
            entry = &obj->vtable24[18];
            ((s32 (*)(void *, s32, s32, u8, s32))entry->func)((u8 *)obj + entry->delta, 0, 0xD, 0xFE, 0);
        }
        func_80049CB4(0x88, obj);
    }
}
