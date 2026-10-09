#include "common.h"

typedef unsigned char u8;

typedef struct Methods800E9F14 {
    u8 pad_00[0x98];
    short delta_98;
    short index_9A;
    void *(*contents_9C)(void *self);
} Methods800E9F14;

/* Partial view: the vtable pointer lives at 0x24. */
typedef struct Obj800E9F14 {
    u8 pad_00[0x24];
    Methods800E9F14 *vtable_24;
} Obj800E9F14;

s32 func_80049CB4(s32 id, ...);
void *func_800CF058(void *target, u8 arg1);

/* Announce the entries of kinds 3, 4 and 9 held in this object's container. */
void func_800E9F14(Obj800E9F14 *o, s32 flag)
{
    void *contents = o->vtable_24->contents_9C((u8 *)o + o->vtable_24->delta_98);
    s32 message;
    void *entry;

    if (contents == 0) {
        return;
    }
    message = flag ? 0x84 : 0x85;
    entry = func_800CF058(contents, 3);
    if (entry != 0) {
        func_80049CB4(message, o, entry);
    }
    entry = func_800CF058(contents, 4);
    if (entry != 0) {
        func_80049CB4(message, o, entry);
    }
    entry = func_800CF058(contents, 9);
    if (entry != 0) {
        func_80049CB4(message, o, entry);
    }
}
