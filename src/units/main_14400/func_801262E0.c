#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

typedef struct Owner Owner;

/* 26-byte bitset indexed by the item's pool D_80143094 index. */
extern u8 D_801CA650[];

/* The owning object is supplied by the method contract but is not read here. */
void func_801262E0(Owner *owner, Item *item)
{
    u32 id = (u8)func_800AFD08(&D_80143094, item);

    if (id != 0xFF) {
        D_801CA650[id >> 3] |= 1 << (id & 7);
    }
}
