#include "common.h"
#include "item_records.h"
#include "map_rect.h"
/* Floor-item grid: a pool handle, then the selected range (s8 +8). */
typedef struct { Handle base; signed char mode; } S;
/* The floor pool D_801430A4, stored in rodata ahead of the D_80154828 vtable. */
extern ItemPool *D_80154824;
extern s32 func_800D4A10(S *);
extern MapPos *func_800D4B60(MapPos *, S *, s32);
extern void func_800AD7E0(Item *, MapPos *, s32);
/* Select a range; range 0 binds the floor pool and places each of its items, clearing
 * item flag bit 5. */
void func_800D4CE4(S *p, signed char mode) {
    s32 i;
    short mask = ~0x20;
    p->mode = mode;
    if (mode == 0) {
        p->base.pool = D_80154824;
        i = func_800D4A10(p) - 1;
        for (;;) {
            Item *it;
            if (i < 0) break;
            it = func_800D4A60(&p->base, i);
            if (it != 0) {
                MapPos buf;
                func_800D4B60(&buf, p, i);
                func_800AD7E0(it, &buf, 1);
                it->flags02 &= mask;
            }
            i--;
        }
    }
}
