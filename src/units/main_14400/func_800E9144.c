#include "common.h"
typedef short s16;
typedef struct { s16 delta; s16 index; void *fn; } VtblEntry;
typedef unsigned char u8;
typedef struct { s32 index; void *container; s32 reverse; u8 *entry; } Iter800E9144;
typedef struct { char pad[0x24]; VtblEntry *vtbl; } Obj800E9144;
Iter800E9144 *func_800CEB20(Iter800E9144 *, void *);
s32 func_800CEBA0(Iter800E9144 *);
u8 *func_800CEC68(Iter800E9144 *);
void func_8010E2CC(u8 *);
/* Item record: byte 0 is the item category, byte 1 the item id. */
static __inline__ s32 notItemA4(u8 *item) {
    return item[1] != 0xA4;
}
s32 func_800E9144(Obj800E9144 *self) {
    Iter800E9144 it;
    s32 found = 0;
    VtblEntry *e = &self->vtbl[19];
    void *list = ((void *(*)(void *))e->fn)((char *)self + e->delta);
    if (list != 0) {
        func_800CEB20(&it, list);
        while (func_800CEBA0(&it)) {
            u8 *item = func_800CEC68(&it);
            s32 hit = item[0] == 8 && notItemA4(item);
            if (hit) {
                func_8010E2CC(item);
                found = 1;
            }
        }
    }
    return found;
}
