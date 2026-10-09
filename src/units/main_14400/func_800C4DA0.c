#include "common.h"

typedef unsigned short u16;

/* Base part initialized by func_800C4840 (owner, short value, count, vtable), then the
 * derived vtable at 0x0C and the linked object at 0x10. */
typedef struct Obj800C4DA0 {
    void *owner_00;
    short value_04;
    s32 count_08;
    void *vtable_0C;
    void *link_10;
} Obj800C4DA0;

/* Initialized original vtable, not BSS. Its full type is unresolved. */
extern unsigned char D_80154000[];

/* Second argument is the owner pointer forwarded from this constructor. */
Obj800C4DA0 *func_800C4840(Obj800C4DA0 *obj, void *a, s32 b);

Obj800C4DA0 *func_800C4DA0(Obj800C4DA0 *obj, void *owner, void *link, u16 value)
{
    func_800C4840(obj, owner, value);
    obj->vtable_0C = D_80154000;
    obj->link_10 = link;
    return obj;
}
