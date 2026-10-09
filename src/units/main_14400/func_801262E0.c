#include "common.h"

typedef unsigned char u8;

typedef struct Owner Owner;
typedef struct Item Item;

/* Id lookup table and a 26-byte bitset indexed by id. */
extern char D_80143094[];
extern u8 D_801CA650[];

s32 func_800AFD08(void *table, void *obj);

/* The owning object is supplied by the method contract but is not read here. */
void func_801262E0(Owner *owner, Item *item)
{
    u32 id = (u8)func_800AFD08(D_80143094, item);

    if (id != 0xFF) {
        D_801CA650[id >> 3] |= 1 << (id & 7);
    }
}
