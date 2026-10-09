#include "common.h"

typedef unsigned char u8;
typedef struct { u8 field00, kind01; u8 pad02[0x2E]; } Item;
typedef Item Obj800BCB18;
/* Complete 0x10-byte pool: two backing pointers, capacity, used count. */
typedef struct { Item *items; u8 *occupied; s32 capacity; s32 used; } Table;
extern u8 D_8014303C;
extern Table D_80143094;
extern void func_800AD3CC(void);
extern Item *func_800AFD78(Table *table, u8 index);
extern s32 func_800AC670(Obj800BCB18 *obj);
extern void func_800AD308(u8 flag);
extern void func_800AD48C(void);
extern void func_800AD650(void);

void func_800ACF60(u8 id) {
    s32 index;
    D_8014303C = id;
    func_800AD3CC();
    index = D_80143094.capacity;
    for (;;) {
        Item *item;
        s32 usable;
        index--;
        if (index == -1) break;
        item = func_800AFD78(&D_80143094, (u8)index);
        if (item != 0) {
            usable = func_800AC670(item);
            usable ^= 1;
            if (usable) func_800AD308(item->kind01);
        }
    }
    func_800AD48C();
    func_800AD650();
}
