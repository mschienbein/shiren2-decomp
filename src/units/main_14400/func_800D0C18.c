#include "common.h"
#include "item_records.h"

typedef unsigned char u8;

/* Item container: pool +0, method table +4, +8 word. */
typedef struct {
    ItemPool *list;
    void **vtable;
    s32 field08;
} Container;

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
    src = func_800D07D8(&self->list, from);
    dst = func_800D07D8(&self->list, to);
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
