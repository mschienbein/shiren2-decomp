#include "common.h"

typedef unsigned char u8;

typedef struct Table Table;
typedef struct Item Item;
typedef struct Entry800D01B8 Entry800D01B8;

/* Embedded entry list appended to by func_800D05E0. */
typedef struct {
    u8 pad0[8];
    Entry800D01B8 *entries;
    s32 count;
    s32 index;
} Obj800D05E0;

typedef struct {
    u8 pad0[0xB0];
    Obj800D05E0 list_B0;
} S;

void *func_800AF818(u32 index);
Item *func_800AFD78(Table *table, u8 index);
void *func_800DAB68(void *item);
void func_800D05E0(Obj800D05E0 *obj, void *arg1, void *arg2);

/* ids[0] selects the table; each following byte adds that table entry to the list at +0xB0. */
void func_800DDC0C(S *s, unsigned char *ids, s32 count)
{
    Table *table;
    s32 remaining;

    table = func_800AF818(*ids++);
    remaining = count;
    for (;;) {
        Item *item;

        if (remaining-- <= 0) {
            break;
        }
        item = func_800AFD78(table, *ids++);
        func_800D05E0(&s->list_B0, func_800DAB68(item), item);
    }
}
