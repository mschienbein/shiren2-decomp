#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { s32 x, y; } Cell;
/* Four-word by-value path handed to the task spawner (func_80085294's Quad). */
typedef struct { u32 from_x, from_y, to_x, to_y; } Path;
typedef struct { u8 pad_00[0x1C]; float scale_x, scale_y; } Unit;
/* 116-byte task record of func_80085294 (layout not needed here). */
typedef struct Task Task;

extern s32 D_8013968C;
void func_80050BAC(s32 owner, u16 flags, Cell *cell, Cell *sub, float scale);
void func_80050CEC(s32 id, s32 arg, Cell *from, Cell *to, float value);
void func_80050F34(s32 id, Cell *cell, s32 direction, s32 arg);
void *func_800A27A4(void *out_direction, void *from, void *to);
s32 func_8007920C(s32 kind, s32 id, s32 x, s32 y);
Unit *func_8007946C(s32 side, s32 slot);
void func_80079560(s32 side, s32 slot, s32 mode);
Task *func_80085294(void (*handler)(Task *task), u32 slot, Path path);
void func_800893D8(Task *task);
void func_800897AC(Task *task);

/* Starts the visual effect of the current action (D_8013968C) from one map
 * cell toward another. */
void func_80050270(Cell *from, Cell *to) {
    Path path;
    Cell copy;
    u8 direction;

    path.from_x = from->y;
    path.from_y = from->x;
    path.to_x = to->y;
    path.to_y = to->x;
    switch (D_8013968C) {
    case 0xED:
        copy.x = from->x;
        copy.y = from->y;
        func_800A27A4(&direction, to, &copy);
        func_80050F34(0x1B7, from, direction, 0);
        return;
    case 0xEE:
        func_80050CEC(0x1BB, 0, from, to, 1.0f);
        return;
    case 0xEF:
        func_80050CEC(0x1BC, 0, from, to, 1.0f);
        return;
    case 0x100:
        if (from != to) func_80050BAC(0x39, 0, from, to, 1.0f);
        return;
    case 0x101:
        func_80050BAC(0x9E, 0x400, from, to, 1.0f);
        return;
    case 0x10D: {
        s32 slot = func_8007920C(0xB, 0xC8, path.from_x, path.from_y);
        if (slot >= 0) {
            Unit *unit = func_8007946C(4, slot);
            func_80079560(4, slot, 1);
            unit->scale_y = 0.4f;
            unit->scale_x = 0.4f;
            func_80085294(func_800897AC, slot, path);
        }
        break;
    }
    case 0x10E:
        func_80050CEC(0x33, 1, from, to, 1.0f);
        return;
    case 0x110: {
        s32 slot = func_8007920C(0xB, 0xC8, path.from_x, path.from_y);
        if (slot >= 0) {
            func_80079560(4, slot, 1);
            func_80085294(func_800897AC, slot, path);
        }
        break;
    }
    case 0x111: {
        s32 slot = func_8007920C(0xC, 0xCA, path.from_x, path.from_y);
        if (slot >= 0) {
            func_80079560(4, slot, 1);
            func_80085294(func_800897AC, slot, path);
        }
        break;
    }
    case 0x112: {
        s32 slot = func_8007920C(0xC, 0xCA, path.from_x, path.from_y);
        if (slot >= 0) {
            func_80079560(4, slot, 1);
            func_80085294(func_800893D8, slot, path);
        }
        break;
    }
    }
}
