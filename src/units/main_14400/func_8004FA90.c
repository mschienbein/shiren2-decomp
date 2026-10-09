#include "common.h"
typedef unsigned char u8;
typedef struct { s32 x, y; } Cell;
typedef struct { u8 pad_00, kind, flags, pad_03[0x1C], type_1F, pad_20[0x64]; s32 id_84; } Entity;
typedef struct { u8 pad_00[0x14]; float height; u8 pad_18[4]; float scale_x, scale_y; } Object;
typedef struct {
    u8 pad_00[0x12]; unsigned short flags; s32 field_14, field_18, time;
    const void *path; s32 target, field_28, mode; u8 pad_30[0x2C];
    s32 x; u8 pad_60[8]; s32 y;
} Effect;
extern s32 D_8013968C;
extern const u8 D_8013F150[], D_8013F178[], D_8013F1D0[];
extern Entity *D_801476B8;
extern s32 func_80048AE4(void), func_800A99D0(void), func_8007920C(s32, s32, s32, s32);
extern void func_80050B3C(s32, Cell *, s32), func_80050E44(s32, Cell *), func_80050F34(s32, Cell *, s32, s32);
extern void *func_80050EB4(s32, Cell *, s32, s32, s32, s32, s32);
extern Object *func_8007946C(s32, s32);
extern void func_80079560(s32, s32, s32), func_80084A20(void), func_80084A68(void), func_80084B80(void);
extern unsigned short func_80084BCC(void);
extern s32 func_80084C00(s32, void (*)(void *), s32, s32, s32);
extern void func_800850F8(void (*)(void *), unsigned short);
extern Effect *func_80085154(void (*)(void *), s32);
extern void *func_800851B0(s32);
extern u8 func_800A8C00(void *);
extern Entity *func_800B4928(Cell *), *func_800B4D80(Cell *), *func_800C5F60(void);
extern void func_80085530(void *), func_800869B8(void *), func_800869E8(void *), func_8008865C(void *), func_800887F0(void *), func_8008A7B0(void *), func_8008AA20(void *), func_8008AEC8(void *), func_8008B004(void *), func_8008B558(void *);
/* Starts the visual/sound effects of the current action (D_8013968C) at a map
 * cell; pos holds the cell in (column, row) order and is zero without a cell. */
void func_8004FA90(Cell *cell) {
    Cell pos;
    if (!cell) { pos.y = 0; pos.x = 0; }
    else { pos.x = cell->y; pos.y = cell->x; }
    switch (D_8013968C) {
    case 0xE4: {
        Entity *entity = func_800B4D80(cell);
        func_80084A68();
        if (func_80084C00(func_80084BCC() & 0xFFFF, func_800887F0, pos.x, pos.y, 1) >= 0) func_800850F8(func_8008865C, 4);
        func_800851B0(0x14);
        func_800850F8(func_8008865C, 6);
        if (entity && (entity->flags & 0x10)) {
            Effect *effect = func_80085154(func_8008B558, 0);
            effect->x = pos.x;
            effect->y = pos.y;
        }
        return;
    }
    case 0xE6: func_80050E44(0x1AF, cell); break;
    case 0xE7:
        func_80084A20(); func_80050F34(0x1CA, cell, 6, 0); func_80084B80(); func_80050E44(0x1C9, cell); break;
    case 0xE8:
        if (func_800B4928(cell)) { func_80084A20(); func_80050E44(0x1C6, cell); func_80084B80(); }
        func_80050E44(0x1C5, cell); break;
    case 0xE9: func_80050E44(0x1CB, cell); break;
    case 0xEA: {
        Entity *entity = func_800B4D80(cell);
        if (entity && entity->kind == 0xDB) {
            Effect *effect = func_80085154(func_80085530, (pos.y % 10) * 11 + pos.x % 11);
            effect->x = pos.x; effect->y = pos.y; effect->time = 5;
        }
        func_80050E44(0x1B6, cell); break;
    }
    case 0xEC: {
        s32 slot = func_800A8C00(func_800C5F60());
        Effect *effect;
        Entity *entity;
        func_80084A68();
        effect = func_80085154(func_8008AA20, slot);
        effect->x = pos.x; effect->y = pos.y;
        entity = func_800B4928(cell);
        if (entity) {
            effect->target = func_800A8C00(entity);
            if (entity->type_1F == 0x17) effect->flags |= 0x4000;
        } else effect->target = -1;
        func_80050E44(0x1BE, cell);
        func_80084A20(); func_80050E44(0x1C0, cell); func_80084B80();
        func_80084A20(); func_800850F8(func_8008865C, 2); func_80050E44(0x1C0, cell); func_80084B80();
        func_800850F8(func_8008865C, 4); func_80050E44(0x1BF, cell); break;
    }
    case 0xEB: {
        Effect *effect = func_80085154(func_80085530, (pos.y % 10) * 11 + pos.x % 11);
        effect->x = pos.x; effect->y = pos.y; effect->time = 0x19;
        func_80050E44(0x1CD, cell); break;
    }
    case 0xF0: func_80050E44(0x1C7, cell); break;
    case 0xF1: func_80050E44(0x1CF, cell); break;
    case 0xF2: func_80050E44(0x1CE, cell); break;
    case 0xF3:
        func_80084A20(); func_80050F34(0x1C4, cell, 6, 0); func_80084B80(); func_80050E44(0x1C3, cell); break;
    case 0xF4:
        func_80084A20(); func_80050F34(0x1BA, cell, 6, 0); func_80084B80(); func_80050E44(0x1B9, cell); break;
    case 0xF5: func_80050B3C(0x1C1, cell, 5); return;
    case 0xF6: func_80050B3C(0x1B8, cell, 5); return;
    case 0xFD:
        func_800850F8(func_8008865C, 7); func_80084A20(); func_80050E44(0x142, cell); func_80084B80();
        func_800850F8(func_8008865C, 0x18); func_80050E44(0x140, cell); break;
    case 0xFE: {
        s32 slot;
        Object *object;
        Effect *effect;
        func_800851B0(0x1A);
        slot = func_8007920C(0, 0x126, pos.x, pos.y);
        object = func_8007946C(4, slot); func_80079560(4, slot, 1);
        object->scale_y = 0.5f; object->scale_x = 0.5f; object->height = 5.0f;
        effect = func_80085154(func_8008B004, slot);
        effect->mode = 4; effect->path = D_8013F150; return;
    }
    case 0xFF: func_80050E44(0x148, cell); break;
    case 0x102: func_800851B0(0xAF); return;
    case 0xFB:
        func_800850F8(func_8008865C, 7); func_80084A20(); func_80050E44(0x142, cell); func_80084B80();
        func_800850F8(func_8008865C, 0x17); func_800851B0(0x7D); return;
    case 0x107:
        if (func_800B4D80(cell)) { Effect *effect = func_80085154(func_8008A7B0, 0); effect->x = pos.x; effect->y = pos.y; }
        return;
    case 0x108: func_800850F8(func_8008865C, 1); return;
    case 0x109: func_80050E44(0x1AF, cell); break;
    case 0x10A:
        if (func_80048AE4()) func_80085154(func_800869E8, 0);
        func_80050E44(0x135, cell); break;
    case 0x10B: {
        Effect *effect;
        func_800851B0(0x21); effect = func_80085154(func_8008AEC8, -1); effect->x = pos.x; effect->y = pos.y; return;
    }
    case 0x10F: func_80050E44(0x134, cell); break;
    case 0x113: func_80050E44(0x141, cell); break;
    case 0x114: func_80050E44(0x9D, cell); break;
    case 0x115: func_80050E44(0x38, cell); break;
    case 0x116: func_80050E44(0x34, cell); break;
    case 0x117: func_80050E44(0x94, cell); break;
    case 0x118: func_80050E44(0xA4, cell); break;
    case 0x119: {
        s32 slot = func_8007920C(0, 0x181, pos.x, pos.y);
        if (slot >= 0) {
            Effect *effect;
            func_80079560(4, slot, 1);
            effect = func_80085154(func_8008B004, slot); effect->mode = 4; effect->path = D_8013F178;
            effect = func_80085154(func_8008B004, slot); effect->mode = 4; effect->path = D_8013F1D0;
        }
        return;
    }
    case 0x11B: func_80050E44(0xA4, cell); break;
    case 0x11C: func_80050E44(0xA9, cell); break;
    case 0x11D: func_800850F8(func_8008865C, 8); func_80050E44(0x142, cell); break;
    case 0x120: func_80050E44(0x1B0, cell); break;
    case 0x121: func_80050EB4(0x1C8, cell, 2, 0, 0, 0, 0x2C); return;
    case 0x123: func_80050E44(0x1BD, cell); break;
    case 0x122:
        if (func_800A99D0()) func_80050E44(0x1AF, cell);
        else {
            func_80084A20(); func_80085154(func_800869B8, -1); func_800851B0(0x1A8);
            func_800850F8(func_8008865C, 0xF); func_800851B0(0xC6); func_800850F8(func_8008865C, 0x20);
            func_800851B0(0xA9); func_80085154(func_800869B8, D_801476B8->id_84); func_80084B80(); func_80050E44(0x1B5, cell);
        }
        break;
    }
}
