#include "common.h"

typedef unsigned char u8;

typedef struct Item801217DC {
    u8 pad_00[0x1E];
    u8 flags_1E;
} Item801217DC;

/* Partial view: only the cell id at 0x2C is used. */
typedef struct Self801217DC {
    u8 pad_00[0x2C];
    u8 cell_2C;
} Self801217DC;

s32 func_800A8C70(u8 id);
void *func_800A8CB0(s32 cell);

static inline s32 cell_valid(u8 cell)
{
    return cell != 0xFF && func_800A8C70(cell) != 0;
}

/* Return the item on this object's cell when the cell is valid and the item has
 * flag bit 4 set, else NULL. */
Item801217DC *func_801217DC(Self801217DC *self)
{
    Item801217DC *item;

    if (cell_valid(self->cell_2C)) {
        item = func_800A8CB0(self->cell_2C);
        if (item != 0 && ((item->flags_1E >> 4) & 1)) {
            return item;
        }
    }
    return 0;
}
