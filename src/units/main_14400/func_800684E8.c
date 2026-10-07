#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;

typedef struct { u8 kind; u8 sub; u8 flags; } Cell;
extern Cell *D_801E02A4;
extern s32 D_801E4E70;
u32 func_80068890(u8 x, u8 y);
u8 func_8006872C(u8 x, u8 y, u32 mask, u32 value);
s32 func_8006262C(s32 x, s32 y);
static inline Cell *cell_at(u32 x, u32 y) { return &D_801E02A4[y * 76 + x]; }
void func_800684E8(u8 x0, u8 y0, u8 x1, u8 y1) {
    u32 x;
    u32 y;
    for (y = y0; y <= y1; y++) {
        Cell *cell;
        x = x0;
        cell = cell_at(x, y);
        for (; x <= x1; x++, cell++) {
            u32 flags = func_80068890(x, y);
            u8 oldKind = cell->kind;
            u8 oldSub = cell->sub;
            cell->flags &= ~2;
            cell->kind = 0;
            cell->sub = 0;
            switch (flags & 0x6080) {
            case 0x4000:
                cell->kind = 1;
                if (flags & 0x20) {
                    cell->flags |= 2;
                    cell->sub = func_8006872C(x, y, 0x4020, 0x4020);
                } else {
                    cell->sub = func_8006872C(x, y, 0x4020, 0x4000);
                }
                break;
            case 0x2000:
                cell->kind = 2;
                cell->sub = func_8006872C(x, y, 0x2000, 0x2000);
                break;
            case 0x80:
                cell->kind = 3;
                cell->sub = func_8006872C(x, y, 0x80, 0x80);
                break;
            default:
                if (D_801E4E70 == 0 || (flags & 0x20100000)) {
                    switch (func_8006262C(x, y)) {
                    case 0xCE:
                        cell->kind = 4;
                        break;
                    case 0xCF:
                        cell->kind = 5;
                        break;
                    }
                }
                break;
            }
            if (cell->kind != oldKind || cell->sub != oldSub) cell->flags |= 1;
        }
    }
}
