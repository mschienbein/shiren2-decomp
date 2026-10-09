#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct {
    s16 delta;
    s16 index;
    void *fn;
} VtEntry800DCB3C;

typedef struct {
    s32 kind;
    void *sender;
    void *arg;
    s32 extra[5];
} Msg800DCB3C;

typedef struct {
    u8 pad0[0x8];
    VtEntry800DCB3C *vtable;
} Receiver800DCB3C;

typedef struct {
    s32 field_0;
    VtEntry800DCB3C *vtable;
    u8 pad8[0x18 - 0x8];
    Receiver800DCB3C *receiver;
} Target800DCB3C;

typedef struct {
    Target800DCB3C *target;
    void *arg;
} Link800DCB3C;

typedef struct {
    u8 pad0[0x8];
    Link800DCB3C link;
} Obj800DCB3C;

extern void *D_801476B8;
void func_800DAD20(Obj800DCB3C *obj, Receiver800DCB3C *receiver);
void func_800DAD80(Obj800DCB3C *obj);
/* Returns the removed item or null; the result is intentionally ignored here. */
void *func_800D02AC(Link800DCB3C *link);

s32 func_800DCB3C(Obj800DCB3C *obj) {
    Link800DCB3C *link = &obj->link;
    Target800DCB3C *target = obj->link.target;
    VtEntry800DCB3C *entry = &target->vtable[12];
    Receiver800DCB3C *receiver;
    Msg800DCB3C msg;
    s32 failed;
    void *arg;
    Msg800DCB3C *m;

    failed = ((s32 (*)(void *, void *, s32))entry->fn)((char *)target + entry->delta, link->arg, 1) != 1;
    if (failed) {
        return 0;
    }
    receiver = obj->link.target->receiver;
    func_800DAD20(obj, receiver);
    arg = link->arg;
    msg.kind = 0xC;
    m = &msg;
    m->sender = D_801476B8;
    m->arg = arg;
    entry = &receiver->vtable[7];
    if (((s32 (*)(void *, void *))entry->fn)((char *)receiver + entry->delta, m)) {
        func_800D02AC(link);
    }
    func_800DAD80(obj);
    return 0;
}
