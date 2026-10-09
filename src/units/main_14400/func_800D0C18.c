#include "common.h"

typedef unsigned char u8;

typedef struct List List;

/* 48-byte list entry (copied whole when entries are shifted). */
typedef struct {
    s32 words[12];
} Item;

typedef struct {
    List *list;
    void **vtable;
    s32 field08;
} Container;

extern u32 func_800D07D8(Container *owner, s32 index);
extern Item *func_800AFD78(List *table, u8 index);
extern void func_800AFCA8(List *table, u8 id);
extern void func_800AFCD8(List *table, u8 id);
extern s32 func_800AF920(List *s, unsigned char n);
extern void func_800AE648(Item *obj);

/* Item-family slot +0x54 override (D_80154668+0x54): void (void *self, s32 source, s32 destination).
 * Moves the entry at position `from` to position `to`, shifting the entries in between; the
 * shift direction compares the positions unsigned (sltu at 0x800D0CBC), as func_800CE87C does. */
void func_800D0C18(Container *self, s32 from, s32 to)
{
    u32 src;
    u32 dst;
    u32 slot;
    s32 step;
    Item *item;
    Item *prev;
    Item saved;

    if (from == to) {
        return;
    }
    src = func_800D07D8(self, from);
    dst = func_800D07D8(self, to);
    item = func_800AFD78(self->list, src);
    saved = *item;
    func_800AFCA8(self->list, src);
    step = -1;
    if ((u32)to < (u32)from) {
        step = 1;
    }
    for (slot = dst; func_800AF920(self->list, slot); slot += step) {
    }
    func_800AFCD8(self->list, slot);
    prev = func_800AFD78(self->list, slot);
    while (slot != dst) {
        slot -= step;
        item = func_800AFD78(self->list, slot);
        *prev = *item;
        func_800AE648(prev);
        prev = item;
    }
    *prev = saved;
    func_800AE648(prev);
}
