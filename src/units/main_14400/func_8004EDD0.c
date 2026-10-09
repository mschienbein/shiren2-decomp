#include "common.h"

typedef struct { s32 x, y; } Pos;
typedef struct {
    Pos pos;
    unsigned char field_08;
    unsigned char field_09;
    unsigned char field_0A;
} Unit;
typedef void (*TaskFn)(void *task);

extern u32 D_8013968C;
unsigned char func_800A8C00(void *actor);
void *func_80085154(TaskFn handler, s32 value);
void *func_800851B0(s32 id);
void func_80050E44(s32 id, Pos *position);
void func_80050F34(s32 id, Pos *position, s32 direction, s32 flags);
void func_800891E0(void *task);
void func_80088FD4(void *task);

static inline void Pos_copy(Pos *dst, Pos *src)
{
    dst->x = src->x;
    dst->y = src->y;
}

/* Starts the effect for the current action id (0xAD..0xB6) on this unit. */
void func_8004EDD0(Unit *unit)
{
    s32 layer = func_800A8C00(unit);
    Pos position;

    switch (D_8013968C) {
    case 0xAD:
    case 0xAE:
    case 0xAF:
    case 0xB0:
    case 0xB1:
        func_80085154(func_800891E0, layer);
        break;
    case 0xB2:
        Pos_copy(&position, &unit->pos);
        func_80050E44(0x37, &position);
        break;
    case 0xB3:
        func_80085154(func_80088FD4, layer);
        func_800851B0(0xAA);
        break;
    case 0xB4: {
        s32 direction = unit->field_08;

        Pos_copy(&position, &unit->pos);
        func_80050F34(0x10E, &position, direction, 0);
        break;
    }
    case 0xB6:
        if (unit->field_0A == 0x10) {
            func_800851B0(0xDA);
        }
        break;
    }
}
