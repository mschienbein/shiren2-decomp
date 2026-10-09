#include "common.h"

typedef unsigned short u16;

/* One window's 14x6 tile grid (0xA8 bytes). */
typedef struct {
    u16 cells[14][6];
} TileGrid;

/* Ten 0x2C-byte window records at 0x801A9080. The draw callback at +0x24 is generic
 * function-pointer storage whose form is selected by the context at +0x28 (see func_800833B8). */
typedef struct {
    u16 state_00;
    u16 active_02;
    u16 field_04;
    u16 x_06;
    u16 y_08;
    u16 width_0A;
    u16 height_0C;
    u16 order_0E;
    u16 dirty_10;
    u16 field_12;
    u16 field_14;
    u16 field_16;
    u16 field_18;
    u16 field_1A;
    TileGrid *tiles_1C;
    u16 field_20;
    u16 pad_22;
    void (*draw_24)(void);
    void *context_28;
} Record;

extern s32 D_8013E818;
extern s32 D_8013E8E0;
extern u16 D_801A9058[10];
extern Record D_801A9080[10];
/* Tile grids indexed by window slot (bss region of 0xD20 bytes; extent not modelled here). */
extern TileGrid D_801A9238[];

/* Open a window in the first free slot; returns the slot or -1. */
s32 func_80081C18(s32 x, s32 y, s32 width, s32 height, void (*draw)(void), void *context) {
    s32 slot, col, i, count;

    for (slot = 0; slot < 10; slot++) {
        Record *record = &D_801A9080[slot];

        if (record->active_02 == 0) {
            record->state_00 = 1;
            record->active_02 = 64;
            record->field_04 = 1;
            record->x_06 = x;
            record->y_08 = y;
            record->width_0A = width;
            record->height_0C = height;
            record->dirty_10 = 0x8000;
            record->field_12 = 0;
            record->field_14 = 0;
            record->draw_24 = draw;
            record->context_28 = context;
            record->field_1A = 0;
            record->tiles_1C = &D_801A9238[slot];
            for (i = 0; i < 14; i++) {
                for (col = 0; col < 6; col++) {
                    record->tiles_1C->cells[i][col] = 0x2FF;
                }
            }
            record->field_20 = 0;
            for (count = 0, i = 0; i < 10; i++) {
                if (D_801A9080[i].active_02) {
                    count++;
                }
            }
            record->order_0E = count;
            D_8013E8E0 = 1;
            D_801A9058[D_8013E818++] = slot;
            return slot;
        }
    }
    return -1;
}
