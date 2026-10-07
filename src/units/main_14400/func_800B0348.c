#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Item Item;
typedef struct { s16 delta; s16 index; void (*func)(Item *, s32); } VtblEntry;
typedef struct { u8 pad0[8]; VtblEntry entry8; } Vtbl;
struct Item { u8 unk0; u8 kind; u8 pad2[6]; Vtbl *vtbl; };
typedef struct { s32 unk0; u8 *bits; s32 count; } Set;
extern u8 D_8015488C[];
Item *func_800AFD78(Set *set, u8 index);
void func_800B0348(Set *set) {
    s32 i = 0;
    for (;;) {
        Item *item;
        if (i >= set->count) {
            break;
        }
        if (set->bits[i >> 3] & D_8015488C[i & 7]) {
            item = func_800AFD78(set, i);
            if (item->kind == 0xF4 && item != 0) {
                item->vtbl->entry8.func((Item *)((u8 *)item + item->vtbl->entry8.delta), 3);
            }
        }
        i++;
    }
}
