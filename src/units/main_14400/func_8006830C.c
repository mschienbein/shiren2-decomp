#include "common.h"

typedef unsigned char u8;

/* 76 x 54 map of 3-byte cells. */
typedef struct { u8 kind; u8 sub; u8 flags; } Cell;
/* Per-cell auto-tile bits and variant index, parallel to the cell map. */
typedef struct { u8 bits; u8 variant; } CellTile;
/* Wall-pattern table: neighbour mask/value and the tile for each of four variants. */
typedef struct { u8 mask; u8 value; u8 tiles[4]; } WallPattern;

extern Cell *D_801E02A4;
extern CellTile D_8016DD24[4104];
extern WallPattern D_8013C4E4[4];
extern s32 D_8016DD18;
extern s32 D_8016DD1C;
extern s32 D_8016DD20;

void func_8006830C(s32 *tile, s32 *overlay, u8 x, u8 y)
{
    Cell *cell = &D_801E02A4[y * 76 + x];
    CellTile *info;
    WallPattern *pattern;
    s32 offset;

    *overlay = 0xFFFF;
    info = &D_8016DD24[y * 76 + x];
    switch (cell->kind) {
    case 0:
        offset = (info->bits >> 6) + 0x200;
        *tile = D_8016DD1C + offset;
        return;
    case 1:
        if (cell->sub == 0xFF) {
            offset = ((info->bits >> 4) & 3) + 0x208;
            *tile = D_8016DD20 + offset;
            return;
        }
        *tile = cell->sub;
        for (pattern = D_8013C4E4; pattern < D_8013C4E4 + 4; pattern++) {
            if ((cell->sub & pattern->mask) == pattern->value) {
                *tile = pattern->tiles[info->variant];
                return;
            }
        }
        return;
    case 2:
    case 3:
        if (cell->sub == 0xFF) {
            *tile = ((info->bits >> 2) & 3) + 0x210;
        } else {
            *tile = cell->sub + 0x100;
        }
        switch (cell->kind) {
        case 2:
            if (cell->sub == 0xFF) {
                *overlay = ((info->bits >> 1) & 1) + 0x214;
            } else {
                *overlay = 0x214;
            }
            break;
        case 3:
            *overlay = (info->bits & 1) + 0x216;
            break;
        }
        return;
    case 4:
        *tile = D_8016DD18 + 0x218;
        return;
    case 5:
        *tile = D_8016DD18 + 0x21A;
        return;
    }
}
