#include "common.h"

typedef unsigned char u8;
typedef struct { s32 x, y; } Cell;
/* Partial view of the 116-byte task record returned by func_80085154; only the
 * fields written here are named. */
typedef struct {
    u8 pad_00[0x24];
    s32 field_24, field_28;
    u8 pad_2C[0x30];
    s32 field_5C, field_60, field_64, field_68, field_6C, field_70;
} Effect;

extern s32 D_8013968C, D_8013979C, D_801397A0, D_801397A4, D_801397A8;
extern Cell *D_801476B8;
Cell *func_800C5F60(void);
void func_80050E44(s32 id, Cell *cell);
void func_80084A20(void);
void func_80084B80(void);
void func_8008B68C(s32 first, s32 second);
void *func_80085154(void (*handler)(void *), s32 value);
void func_8008B500(void *task);
void func_8008B558(void *task);
void func_8008B600(void *task);
void func_8008B63C(void *task);
void func_8008B72C(void *task);
void func_8008B768(void *task);

/* Starts the area effect of the current action (D_8013968C) at a map cell. */
void func_800507F4(Cell *cell) {
    Cell pos;

    if (cell) {
        pos.x = cell->y;
        pos.y = cell->x;
    }
    switch (D_8013968C) {
    case 0xD7:
    case 0xD8: {
        Effect *effect = func_80085154(D_8013968C == 0xD7 ? func_8008B558 : func_8008B500, 0);
        effect->field_5C = pos.x;
        effect->field_68 = pos.y;
        effect->field_64 = D_801476B8->y;
        effect->field_70 = D_801476B8->x;
        effect->field_24 = func_800C5F60()->y;
        effect->field_28 = func_800C5F60()->x;
        break;
    }
    case 0xE2: {
        Effect *effect = func_80085154(func_8008B600, 0);
        effect->field_5C = pos.x;
        effect->field_68 = pos.y;
        break;
    }
    case 0xE3: {
        /* Also widens the bounding box of affected cells. */
        Effect *effect = func_80085154(func_8008B63C, 0);
        effect->field_5C = pos.x;
        effect->field_68 = pos.y;
        if (D_8013979C > pos.x) D_8013979C = pos.x;
        if (D_801397A4 < pos.x) D_801397A4 = pos.x;
        if (D_801397A0 > pos.y) D_801397A0 = pos.y;
        if (D_801397A8 < pos.y) D_801397A8 = pos.y;
        break;
    }
    case 0xE0:
        func_80084A20();
        func_80050E44(0x142, cell);
        func_80084B80();
        func_80085154(func_8008B768, 0);
        func_80050E44(0x145, cell);
        break;
    case 0xDF:
        func_8008B68C(pos.x, pos.y);
        break;
    case 0xE1: {
        Effect *effect = func_80085154(func_8008B72C, 0);
        effect->field_5C = pos.x;
        effect->field_68 = pos.y;
        break;
    }
    }
}
