#include "common.h"

typedef struct { s32 unk0; s32 unk4; } Item;
typedef signed char s8;
typedef struct ShirenDirection { s8 value; } ShirenDirection;
typedef struct { Item point; ShirenDirection kind; s32 end; s32 cur; } Iter;
extern void func_800C25D0(Iter *, const Item *, const ShirenDirection *, s32);
extern Item *func_800C2758(Item *, Iter *);
extern u32 func_800B1C6C(void *);
s32 func_800BB474(void *self, const Item *key, ShirenDirection kind, s32 limit) {
    Iter it;
    Item item;
    Iter *iter = &it;
    unsigned char n;
    func_800C25D0(iter, key, &kind, limit);
    n = 0;
    while (1) {
        if (iter->cur >= iter->end) break;
        func_800C2758(&item, iter);
        if (func_800B1C6C(&item) & 0x800) break;
        n++;
    }
    return n ? n - 1 : 0;
}
