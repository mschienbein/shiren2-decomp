#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

#define NULL ((void *)0)

typedef struct {
    s16 delta;
    s16 pad2;
    u32 (*func)(void *); /* slot 13 (+0x68/+0x6C): u32 func_800E0E88(Obj *) */
} VtblEntry;

typedef struct {
    u8 pad0[0x1E];
    u8 flags;
    u8 pad1F[5];
    VtblEntry *vtbl;
} Unit;

extern u8 D_80147620[];
s32 func_800C5954(void *, u16, u16);
s32 func_800E8DC8(Unit *, s16);

/* arg0 is passed by the caller contract but not read here. */
s32 func_8010E5FC(void *arg0, Unit *unit) {
    s32 value = func_800C5954(D_80147620, 10, 20);
    s32 result;

    if (unit != NULL) {
        if (unit->flags & 0xC) {
            value = func_800E8DC8(unit, (s16)value);
        } else if (unit->flags & 0x7C) {
            value += unit->vtbl[13].func((u8 *)unit + unit->vtbl[13].delta);
        }
    }
    result = 1;
    if ((s16)value >= 0) {
        result = (s16)value;
    }
    return result;
}
