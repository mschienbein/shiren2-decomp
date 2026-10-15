#include "common.h"
#include "item_records.h"

typedef unsigned char u8;
extern u8 D_8014303C;
extern void func_800AD3CC(void);
extern s32 func_800AC670(void *key);
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
            if (usable) func_800AD308(item->field01);
        }
    }
    func_800AD48C();
    func_800AD650();
}
