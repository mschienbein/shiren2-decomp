#include "common.h"

typedef struct { unsigned char fields_00[0x1C]; unsigned short field_1C; } Item;
extern unsigned short D_801569D0;
extern s32 func_80049CB4(s32, ...);
extern void func_800498E4(s32, ...);
extern Item *func_800A6CF0(void *);
extern s32 func_800A674C(void *, Item *);
extern u32 func_800B1C6C(Item *);
extern char *func_800A3B20(Item *);
extern s32 func_800E1CC4(void *, s32);
extern void func_80117B00(void *, void *, Item *, short);
static inline s32 blocked(Item *item) { return func_800B1C6C(item) & 0x4000; }
void func_80117BC0(void *state, void *actor) {
    Item *item;
    func_80049CB4(0x46);
    item = func_800A6CF0(actor);
    {
    s32 allowed = 0;
    if (func_800A674C(actor, item) && !(item->field_1C & 1)) allowed = blocked(item) == 0;
    if (allowed && item) {
        s32 scale;
        func_800498E4(0x11E, func_800A3B20(item));
        scale = func_800E1CC4(actor, 3) ? 2 : 1;
        func_80117B00(state, actor, item, (short)(D_801569D0 * scale));
    }
    }
}
