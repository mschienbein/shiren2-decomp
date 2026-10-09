#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct Item Item;

/* Item method-table entry (g++ {delta, index, pfn}); slot +0x30/+0x34 is the name
 * formatter char *(Item *self, char *msg). */
typedef struct {
    s16 delta;
    s16 index;
    char *(*func)(Item *self, char *msg);
} ItemVtEntry;

struct Item {
    u8 unk0;
    u8 id;
    u8 pad2[6];
    ItemVtEntry *vtable;
};

typedef struct {
    Item *item;
} Holder;

char *func_800A2154(Holder *holder, char *msg)
{
    Item *item = holder->item;
    return item->vtable[6].func((Item *)((u8 *)item + item->vtable[6].delta), msg);
}
