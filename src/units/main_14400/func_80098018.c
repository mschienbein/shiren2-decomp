#include "common.h"

typedef unsigned char u8;
typedef short s16;

/* Container/item link filled by func_800D01B8. */
typedef struct {
    void *container;
    void *item;
} Pair;

typedef struct { s32 x; s32 y; } Pos;

/* Item-set table (vptr at +4): slot +0x3C returns the item at an index. */
typedef struct { s16 delta, index; void *(*call)(void *self, u32 index); } ItemSlot;
typedef struct { u8 pad_00[0x38]; ItemSlot item; } ItemSetVtbl;
typedef struct {
    s32 unk_00;
    ItemSetVtbl *vtbl;
} ItemSet;

/* Widget family (root D_80151E38) slot +0x7C: s32 (void *self, Pos *pos). */
typedef struct { s16 delta, index; s32 (*call)(void *self, Pos *pos); } PosSlot;
typedef struct { u8 pad_00[0x78]; PosSlot row; } WidgetVtbl;

/* Menu over three item sets: rows [limits[i], limits[i + 1]) belong to sets[i]. */
typedef struct {
    u8 pad_00[0x34];
    Pos cursor;
    u8 pad_3C[0x10];
    WidgetVtbl *vtable_4C;
    u8 pad_50[0x2A0];
    ItemSet *sets[3];
    u8 pad_2FC[0xB8];
    s32 limits[4];
} Menu;

extern void *func_800D0180(void *pair);
extern void func_800D01B8(void *pair, void *target, void *item);

/* Returns the link to the item on row `row` (empty when the row is out of range). */
Pair func_80098018(Menu *menu, s32 row)
{
    Pair link;
    s32 i;

    func_800D0180(&link);
    /* ODD_C: groups the row lookup; a negative row breaks out to the shared empty-link return.
     * Also shapes scheduling: the if-guarded loop and goto-done forms each differ in 9 words. */
    do {
        if (row < 0) break;
        for (i = 0; ; i++) {
            ItemSet *set;

            if (i >= 3) break;
            if (row < menu->limits[i + 1]) {
                set = menu->sets[i];
                func_800D01B8(&link, menu->sets[i],
                    set->vtbl->item.call((u8 *)set + set->vtbl->item.delta, row - menu->limits[i]));
                return link;
            }
        }
    } while (0);
    func_800D01B8(&link, 0, 0);
    return link;
}
