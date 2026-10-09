#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s32 col;
    s32 row;
} Cursor8009B374;

typedef struct {
    s16 delta;
    s16 index;
    void (*fn)(void *self, Cursor8009B374 *cursor);
} VtEntry8009B374;

typedef struct {
    u8 pad0[0x34];
    Cursor8009B374 cursor;
    u8 pad3C[0x4C - 0x3C];
    VtEntry8009B374 *vtable;
} Menu8009B374;

extern s32 D_80143110;
extern u8 D_801528C0[];
extern s32 D_801528C4[];
void func_80045A24(s32 sound);

void func_8009B374(Menu8009B374 *menu, s32 dir) {
    Cursor8009B374 cursor = menu->cursor;
    s32 lastRow = (D_80143110 - 1) / 6;
    s32 lastCount = D_80143110 - lastRow * 6;
    s32 page;
    s32 prev;
    VtEntry8009B374 *entry;

    if (lastRow >= 4) {
        lastRow = 3;
    }
    switch (dir) {
        case 0: {
            s32 col;
            s32 canMove;
            if (cursor.row > lastRow) {
                cursor.row = lastRow;
            }
            col = cursor.col;
            canMove = cursor.row == lastRow ? col < lastCount : col < 6;
            if (canMove) {
                cursor.col++;
            } else {
                cursor.col = cursor.row == 1 || cursor.row == 2;
            }
            break;
        }
        case 1: {
            s32 col;
            s32 canMove;
            if (cursor.row > lastRow) {
                cursor.row = lastRow;
            }
            col = cursor.col;
            canMove = cursor.row == 1 || cursor.row == 2 ? col >= 2 : col > 0;
            if (canMove) {
                cursor.col--;
            } else {
                cursor.col = cursor.row == lastRow ? lastCount : 6;
            }
            break;
        }
        case 2:
            if (cursor.col != 0) {
                if (cursor.row < lastRow) {
                    cursor.row++;
                    if (cursor.row == lastRow && cursor.col > lastCount) {
                        cursor.col = lastCount;
                    }
                } else {
                    cursor.row = 0;
                }
            } else {
                /* ODD_C: one local carries the next page and then that page's first
                   row; the ROM loads each value straight into it (no separate temp),
                   which keeps this tail distinct from case 3's. */
                page = D_801528C0[menu->cursor.row];
                page++;
                if (page >= 2) {
                    page = 0;
                }
                page = D_801528C4[page];
                cursor.row = page;
            }
            break;
        case 3:
            if (cursor.col != 0) {
                if (cursor.row > 0) {
                    cursor.row--;
                } else {
                    cursor.row = lastRow;
                    if (cursor.col > lastCount) {
                        cursor.col = lastCount;
                    }
                }
            } else {
                prev = D_801528C0[menu->cursor.row] - 1;
                if (prev < 0) {
                    prev = 1;
                }
                cursor.row = D_801528C4[prev];
            }
            break;
    }
    if (cursor.row != menu->cursor.row || cursor.col != menu->cursor.col) {
        entry = &menu->vtable[16];
        entry->fn((char *)menu + entry->delta, &cursor);
        func_80045A24(2);
    }
}
